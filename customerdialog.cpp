#include "customerdialog.h"
#include "ui_customerdialog.h"
#include "sale.h"
#include <QDebug>
#include <QSettings>
#include <QScrollBar>

CustomerDialog::CustomerDialog(QMainWindow *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerDialog)
{
    ui->setupUi(this);
    this->updateBack(0);
    this->updateTotal(0);
    this->m = m;

}

CustomerDialog::~CustomerDialog()
{
    delete ui;
}

QTreeView* CustomerDialog::getTreeView() {
    //return this->ui->treeView;
}
void CustomerDialog::setModel(Sale* s) {
    this->ui->treeView->setModel(s);
    ui->treeView->hideColumn(0);

    QObject::connect(s, SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(scrollToLast()));

    QSettings settings("settings.ini",QSettings::IniFormat);
    ui->treeView->setColumnWidth(1, settings.value("customerDialog/articleWidth",500).toInt() );
    ui->treeView->setColumnWidth(2, settings.value("customerDialog/quantityWidth",100).toInt() );
    ui->treeView->setColumnWidth(3, settings.value("customerDialog/singlePriceWidth",100).toInt() );
    ui->treeView->setColumnWidth(4, settings.value("customerDialog/sumPriceWidth",100).toInt() );
}

void CustomerDialog::scrollToLast() {
    ui->treeView->scrollToBottom();
}

bool CustomerDialog::focusInEvent(QEvent *event)
{
    qDebug() << "setting the focus to main window again";
    this->m->setFocus();

    return false;
}


void CustomerDialog::updateTotal(float total) {
    ui->lblToPay->setText(QString::number(total,'f',2) + " €");
}

void CustomerDialog::updateBack(float change) {
    ui->lblChange->setText(QString::number(change,'f',2) + " €");
}
