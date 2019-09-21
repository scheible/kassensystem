#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "article.h"
#include "articlefactory.h"
#include "sale.h"
#include "quickbutton.h"
#include "categorybuttons.h"
#include "freearticle.h"
#include "changequantity.h"
#include "paydialog.h"
#include "reporting.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopWidget>
#include "searchcustomer.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QSettings settings("settings.ini",QSettings::IniFormat);
    this->databaseFileName = settings.value("main/databasefilename","database.db").toString();
    this->showWarningOnArticleNotFound = settings.value("main/showwarningonarticlenotfound",true).toBool();

    //Show the customer display
    this->customerDialog = new CustomerDialog(this,this);
    this->customerDialogMode = settings.value("customerDialog/mode",0).toInt();

    int display = settings.value("customerDialog/display",1).toInt();
    QDesktopWidget dw;
    if (dw.screenCount() > display && this->customerDialogMode != 0) {  //the monitor exists
        QRect screenres = dw.screenGeometry(display);
        this->customerDialog->move(QPoint(screenres.x(), screenres.y()));
    }

    if (this->customerDialogMode == 0) {
        ui->btnToggleCustomerDisplay->hide();
    }

    if (this->customerDialogMode == 1) {
        this->customerDialog->show();
    } else if (this->customerDialogMode == 2) {
        this->customerDialog->showFullScreen();
    }
    // call the method setFocus otherwise the focusIn FocusOut events are not called for some reason?!
    this->customerDialog->setFocus();
    this->setFocus();


    // Open the database file
    QMessageBox messageBox;
    QFileInfo databaseFile(this->databaseFileName);
    if (databaseFile.exists() && databaseFile.isFile())
    {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(databaseFile.absoluteFilePath());
        if (this->db.open()) {
            qDebug() << "opened database successfully";
        } else {
            qDebug() << "opening database failed";           
            messageBox.critical(0,"Error","Datenbank konnte nicht geöffnet werden");
        }
    } else {
        qDebug() << "database file " << databaseFile.path() << databaseFile.fileName() << " not found";
        messageBox.critical(0,"Error","Datenbankdatei wurde nicht gefunden. Dies kann daran liegen, dass die Datei " + this->databaseFileName + " gelöscht, verschoben oder umbenannt wurde!");
        return;
    }

    // A sale object represents a list of articles that a customer wants to buy
    // the actually on the GUI presented sale is stored in sale. Each sale is
    // stored in salesQueue and the index is saved in currentSaleIndex. The user
    // jump forward and backwards to view old sales.
    sale = new Sale();
    currentSaleIndex = 0;
    salesQueue.prepend(sale);
    hookSale(sale); //Connect the sale object to the GUI

    artFact = new ArticleFactory(this->databaseFileName);

    catButtons = new CategoryButtons(this);
    catButtons->setupButtons(ui->gridLayout);
    catButtons->setScrollBar(ui->verticalScrollBar);
    catButtons->openCategory(0);

    QObject::connect(ui->pushButton_3,SIGNAL(clicked(bool)),catButtons,SLOT(goBack()));
    QObject::connect(catButtons,SIGNAL(addArticlePlu(int,float)),this,SLOT(addArticlePlu(int,float)));

    QList<int> list;
    list.append(3*width()/4);
    list.append(width()/4);
    ui->splitter_2->setSizes(list);

    //On startup there is only one sale object active (the current one). There is no need
    //to go back to another sale
    ui->btnSaleBack->setEnabled(false);
    ui->btnSaleNext->setEnabled(false);
    ui->btnJumpNewestSale->setEnabled(false);

    ui->treeView->setColumnWidth(0, settings.value("salelist/pluWidth",50).toInt() );
    ui->treeView->setColumnWidth(1, settings.value("salelist/articleWidth",500).toInt() );
    ui->treeView->setColumnWidth(2, settings.value("salelist/quantityWidth",60).toInt() );
    ui->treeView->setColumnWidth(3, settings.value("salelist/singlePriceWidth",60).toInt() );
    ui->treeView->setColumnWidth(4, settings.value("salelist/sumPriceWidth",60).toInt() );

    if (settings.value("main/fullscreen",false).toBool()) {this->showFullScreen();}

    this->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::deleteArticle() {

}
void MainWindow::showMessage(QString msg) {
    //There is a specific label on the GUI for generic messages
    //set it with this method
    ui->lblMessage->setText(msg);
}
void MainWindow::addArticle(Article *art, float quantity) {
    /* Info regarding takeBackArticle:
     * A takeback article is an article that is normally not sold
     * but taken back (i.e. bought from the customer). E.g. an empty
     * crate with deposit.
     *
     * In effect the quantity 1 means that the price is negative
     *
     * In the database is marked if an article is an article that is
     * usually taken back. If the article is actually sold you have
     * to use quantity -1 to result in a postive price.
     *
     */

    if (sale->isLocked()) {
        art->deleteThis();
        this->showMessage("Vorgang ist nicht Editierbar. Klicke auf Akt. Vorgang!");
        return;
    }

    Article *existingArticle = sale->getArticlePLU(art->getPLU());
    if (existingArticle) {
        existingArticle->addQuantity(quantity);
        art->deleteThis();
    } else {
        Article* pfand = 0;
        if (art != 0) {
            //Add the article to the sale object
            sale->addArticle(art);
            //select the article in the tree view
            QModelIndex next_index = ui->treeView->model()->index(ui->treeView->model()->rowCount()-1, 0);
            ui->treeView->setCurrentIndex(next_index);

            //If the article has a deposit, create a article for that
            if (art->hasDeposit()) {
                pfand = artFact->newPLUArticleFromDb(art->getDepositPLU());
                if (pfand != 0) {
                    pfand->markAsDeposit();
                    //add to the list
                    sale->addArticle(pfand);

                    //connect the deposit to the article
                    //this is important to make the quantity change simultaniously
                    QObject::connect(art,SIGNAL(quantityChanged(float)),pfand,SLOT(setQuantity(float)));
                    QObject::connect(art,SIGNAL(dueToDelete()),pfand,SLOT(deleteThis()));
                }
            }
            art->setQuantity(quantity);
        }
    }
}

void MainWindow::addArticlePlu(int plu, float quantity) {
    Article* art;
    art = artFact->newPLUArticleFromDb(plu);

    if (art) {
        this->addArticle(art,quantity);
    } else {
        this->showMessage("PLU nicht gefunden!");

        if (this->showWarningOnArticleNotFound) {
            QMessageBox messageBox;
            messageBox.warning(0,"Achtung!","PLU nicht gefunden!");
        } else {
            QApplication::beep();
        }
    }
}
void MainWindow::addArticleEAN(QString ean, float quantity) {
    // Handle with care: not tested at all!!!
    Article* art;
    art = artFact->newBarcodeArticleFromDb(ean);

    if (art) {
        this->addArticle(art,quantity);
    } else {
        this->showMessage("EAN Code nicht gefunden.");
        if (this->showWarningOnArticleNotFound) {
            QMessageBox messageBox;
            messageBox.warning(0,"Achtung!","EAN Code nicht gefunden!");
        } else {
            QApplication::beep();
        }
    }
}
void MainWindow::on_btnAddArticle_clicked()
{
    if (ui->txtPlu->text().length() >= 8) {
        //Barcode was scanned
        QString ean = ui->txtPlu->text();
        this->addArticleEAN(ean);
    } else {
        int plu = ui->txtPlu->text().toInt();
        float quantity = ui->txtQuantity->text().replace(',','.').toFloat();

        if (quantity == 0) {
            quantity = 1;
        }

        if (plu == 0) {
            FreeArticle *f = new FreeArticle(this);
            //TODO: make sure that the FreeArticle Object is destroyed correctly
            f->show();
        } else {
            addArticlePlu(plu,quantity);
        }
    }
    ui->txtPlu->clear();
    ui->txtQuantity->clear();
    ui->txtPlu->setFocus();
}
void MainWindow::on_btnDeleteArticle_clicked()
{
     QModelIndexList i = ui->treeView->selectionModel()->selectedRows();
     if (i.length() > 0) {
        Article *art = static_cast<Article *>(i.at(0).internalPointer());
        art->deleteThis();
     }
}
void MainWindow::updateTotal(float total) {
    ui->lblPriceTotal->setText( QString::number(total,'f',2) + " €" );
}
void MainWindow::updateTime(QDateTime datetime) {
    ui->lblTimestampStart->setText( datetime.toString("hh:mm:ss - dd.MM.yy"));
}

void MainWindow::updateArticleCount(float count)
{
    ui->lblArticleCountValue->setText(QString::number(count));
}

void MainWindow::updateTakebackCount(float count)
{
    ui->lblTakebackCountValue->setText(QString::number(count));
}
void MainWindow::on_txtSearch_returnPressed()
{
    if (ui->txtSearch->text() != "") {
        catButtons->search(ui->txtSearch->text());
    } else {
        catButtons->openCategory(0);
    }

}
void MainWindow::unhookSale(Sale* s) {
    //Disconnect a sale object from the GUI
    QObject::disconnect(s,SIGNAL(articleDeleted()),this,SLOT(deleteArticle()));
    QObject::disconnect(s,SIGNAL(newMessage(QString)),this,SLOT(showMessage(QString)));
    //QObject::disconnect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),sale,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    QObject::disconnect(s,SIGNAL(totalChanged(float)),this,SLOT(updateTotal(float)));
    QObject::disconnect(s,SIGNAL(articleCountChanged(float)),this,SLOT(updateArticleCount(float)));
    QObject::disconnect(s,SIGNAL(takebackCountChanged(float)),this,SLOT(updateTakebackCount(float)));
    QObject::disconnect(s,SIGNAL(startTimeIsUpdated(QDateTime)),this,SLOT(updateTime(QDateTime)));
    QObject::disconnect(s,SIGNAL(totalChanged(float)),this->customerDialog,SLOT(updateTotal(float)));

    this->customerDialog->updateBack(0);
    this->customerDialog->updateTotal(0);
}
void MainWindow::hookSale(Sale* s) {
    //connect the sale object to the GUI. This means that all changes in the sale object are instantly reflected
    //to the GUI. Make sure to unhook it before hooking another sale!

    //show sale in the tree view
    ui->treeView->setModel(s);
    customerDialog->setModel(s);

    //connect the signals to the new sale
    QObject::connect(s,SIGNAL(articleDeleted()),this,SLOT(deleteArticle()));
    QObject::connect(s,SIGNAL(newMessage(QString)),this,SLOT(showMessage(QString)));
    //QObject::connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),sale,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(s,SIGNAL(totalChanged(float)),this,SLOT(updateTotal(float)));
    QObject::connect(s,SIGNAL(articleCountChanged(float)),this,SLOT(updateArticleCount(float)));
    QObject::connect(s,SIGNAL(takebackCountChanged(float)),this,SLOT(updateTakebackCount(float)));
    QObject::connect(s,SIGNAL(startTimeIsUpdated(QDateTime)),this,SLOT(updateTime(QDateTime)));
    QObject::connect(s,SIGNAL(totalChanged(float)),this->customerDialog,SLOT(updateTotal(float)));

    updateTime(sale->getTimestampStart());
    updateTotal(sale->getTotal());
    updateArticleCount(sale->getArticleCount());
    updateTakebackCount(sale->getTakebackCount());
    this->customerDialog->updateTotal(sale->getTotal());
    showMessage("");
}
void MainWindow::newSale() {

    // only create a new sales process
    // if it is on the most recent process
    if (currentSaleIndex == 0) {
        unhookSale(sale);

        //create a new sales object
        sale = new Sale();
        salesQueue.prepend(sale);
        currentSaleIndex = 0;

        hookSale(sale);

        catButtons->openCategory(0);
        ui->btnSaleBack->setEnabled(true);
    } else {
        on_btnJumpNewestSale_clicked();
    }
}
void MainWindow::on_btnCategoryGoToRoot_clicked()
{
    catButtons->openCategory(0);
    ui->txtSearch->clear();
}
void MainWindow::on_btnSaleBack_clicked()
{
    //check if there is a previous sale
    if (salesQueue.length() > (currentSaleIndex+1)) {
        currentSaleIndex++;

        unhookSale(sale);

        sale = salesQueue.at(currentSaleIndex);

        hookSale(sale);

        ui->btnJumpNewestSale->setEnabled(true);
        ui->btnSaleNext->setEnabled(true);

        if (salesQueue.length()-1 == currentSaleIndex) {
            ui->btnSaleBack->setDisabled(true);
        }
    }
}
void MainWindow::on_btnSaleNext_clicked()
{
    //check if it is not the most current sale
    if (currentSaleIndex > 0) {
        unhookSale(sale);
        currentSaleIndex--;
        sale = salesQueue.at(currentSaleIndex);
        hookSale(sale);

        if (currentSaleIndex <= 0) {
            ui->btnSaleNext->setDisabled(true);
            ui->btnJumpNewestSale->setDisabled(true);
        }
        ui->btnSaleBack->setEnabled(true);
    }
}
void MainWindow::on_btnPay_clicked()
{
    PayDialog *p = new PayDialog(this,this);
    p->show();
    //TODO: Make sure that the PayDialog object is destroyed properly
}
void MainWindow::on_btnJumpNewestSale_clicked()
{
    if (salesQueue.length() > 0) {
        unhookSale(sale);
        currentSaleIndex = 0;
        sale = salesQueue.at(0);
        hookSale(sale);
    }

    ui->btnJumpNewestSale->setDisabled(true);
    ui->btnSaleNext->setDisabled(true);
    ui->btnSaleBack->setEnabled(true);
}
void MainWindow::on_btnQuantityDec_clicked()
{
    QModelIndexList i = ui->treeView->selectionModel()->selectedRows();
    if (i.length() > 0) {
        Article *art = static_cast<Article *>(i.at(0).internalPointer());
        art->addQuantity(-1);
    }
}
void MainWindow::on_btnQuantityInc_clicked()
{
    QModelIndexList i = ui->treeView->selectionModel()->selectedRows();
    if (i.length() > 0) {
        Article *art = static_cast<Article *>(i.at(0).internalPointer());
        art->addQuantity(1);
    }
}
void MainWindow::on_btnQuantityEnter_clicked()
{
    QModelIndexList i = ui->treeView->selectionModel()->selectedRows();
    if (i.length() > 0) {
        Article *art = static_cast<Article *>(i.at(0).internalPointer());
        changeQuantity *c = new changeQuantity(art,this);
        c->show();
        //TODO: Make Sure the changeQuantity Window is destroyed properly
    }
}
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->isActiveWindow() || this->customerDialog->isActiveWindow()) {
        if (event->key() == Qt::Key_B && !ui->txtSearch->hasFocus()) {
            on_btnPay_clicked();
        } else if (event->key() == Qt::Key_0 ||
                   event->key() == Qt::Key_1 ||
                   event->key() == Qt::Key_2 ||
                   event->key() == Qt::Key_3 ||
                   event->key() == Qt::Key_4 ||
                   event->key() == Qt::Key_5 ||
                   event->key() == Qt::Key_6 ||
                   event->key() == Qt::Key_7 ||
                   event->key() == Qt::Key_8 ||
                   event->key() == Qt::Key_9) {
            if (!ui->txtQuantity->hasFocus() && !ui->txtSearch->hasFocus()) {
                ui->txtPlu->setFocus();
            }
        } else if(event->key() == Qt::Key_M && !ui->txtSearch->hasFocus()) {
            on_btnQuantityEnter_clicked();
        } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
            ui->treeView->setFocus();
        } else if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
            ui->treeView->setFocus();
        }
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (this->isActiveWindow() || this->customerDialog->isActiveWindow()) {

        if(event->key() == Qt::Key_Delete) {
            on_btnDeleteArticle_clicked();
        } else if(event->key() == Qt::Key_Plus) {
            on_btnQuantityInc_clicked();
        } else if(event->key() == Qt::Key_Minus) {
            on_btnQuantityDec_clicked();
        } else if(event->key() == Qt::Key_V) {
            if (this->isFullScreen()) {
                this->showNormal();
            } else {
                this->showFullScreen();
            }
        } else if (event->key() == Qt::Key_Left) {
            on_btnSaleBack_clicked();
        } else if (event->key() == Qt::Key_Right) {
            on_btnSaleNext_clicked();
        } else if(event->key() == Qt::Key_S && !ui->txtSearch->hasFocus()) {
            ui->txtSearch->setFocus();
            ui->txtSearch->clear();
        } else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            if (!ui->txtSearch->hasFocus() && ui->txtPlu->text() != "") {
                this->on_btnAddArticle_clicked();
            }
        }
    }
}
void MainWindow::on_btnCustomer_clicked()
{
    SearchCustomer *s = new SearchCustomer(sale);
    s->show();
    //TODO: Make sure the searchCustomer window is destroyed properly
}
void MainWindow::on_btnDeleteAll_clicked()
{
    if (!sale->isLocked()) {
        sale->clear();
        ui->lblTimestampStart->clear();
        this->showMessage("Alle Artikel entfernt");
    } else {
        this->showMessage("Verkaufsvorgang nicht mehr editierbar. Gehe auf Akt. Vorgang!");
    }
}
void MainWindow::on_btnCalculator_clicked()
{
    system("calc");
}
void MainWindow::on_btnReporting_clicked()
{
    Reporting *rtp = new Reporting();
    rtp->show();
}

void MainWindow::on_btnToggleCustomerDisplay_toggled(bool checked)
{
    if (ui->btnToggleCustomerDisplay->isChecked()) {
        qDebug("Customer Display on");
        if (this->customerDialogMode == 1) {
            this->customerDialog->show();
        } else if (this->customerDialogMode == 2) {
            this->customerDialog->showFullScreen();
        }
    } else {
        qDebug("Custoerm Display off");
        this->customerDialog->hide();
    }
}
