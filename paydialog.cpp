#include "paydialog.h"
#include "ui_paydialog.h"
#include <QDebug>
#include "bonprinter.h"

PayDialog::PayDialog(MainWindow *w, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayDialog)
{
    ui->setupUi(this);
    mWindow = w;

    QString sToPay;
    sToPay = QString::number(mWindow->sale->getTotal(),'f',2) + " €";
    ui->txtToPay->setText(sToPay);
    ui->btnNew->setVisible(false);

    if (mWindow->sale->getGivenMoney() != 0) {
        ui->txtGiven->setText(QString::number(mWindow->sale->getGivenMoney()*100));
    }

    red.setColor(QPalette::Text,Qt::red);
    black.setColor(QPalette::Text,Qt::black);
}

PayDialog::~PayDialog()
{
    delete ui;
}

void PayDialog::on_btnCancel_clicked()
{
    this->close();
    delete this;
}

void PayDialog::on_btnOkWithBon_clicked()
{
    this->on_btnOkWithoutBon_clicked();
    BonPrinter *bp = new BonPrinter(this->mWindow->sale);
    bp->print();
    delete bp;
}

void PayDialog::on_btnOkWithoutBon_clicked()
{
    mWindow->sale->pay(givenMoney);
    ui->btnNew->setVisible(true);
    ui->btnOkWithBon->setVisible(false);
    ui->btnOkWithoutBon->setVisible(false);
    ui->txtGiven->setDisabled(true);
    ui->btnCancel->setDisabled(true);

}

void PayDialog::on_txtGiven_returnPressed()
{
    ui->btnOkWithoutBon->setFocus();
}

void PayDialog::on_txtGiven_editingFinished()
{

}

void PayDialog::on_txtGiven_textChanged(const QString &arg1)
{
    QString sGiven = arg1;
    sGiven.replace('.',"");
    sGiven.replace(" €","");
    float fGiven = sGiven.toFloat();

    fGiven = fGiven / 100;
    givenMoney = fGiven;
    change = fGiven - mWindow->sale->getTotal();

    ui->txtGiven->setText( QString::number(fGiven,'f',2) + " €" );
    ui->txtBack->setText( QString::number(change,'f',2) + " €");
    int p = ui->txtGiven->text().length() - 2;
    ui->txtGiven->setCursorPosition(p);
}

void PayDialog::on_btnNew_clicked()
{
    mWindow->newSale();
    this->close();
    delete this;
}

void PayDialog::on_txtBack_textChanged(const QString &arg1)
{
    if (change < 0) {
        ui->txtBack->setPalette(red);

    } else {
         ui->txtBack->setPalette(black);
    }
}
