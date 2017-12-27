#include "changequantity.h"
#include "ui_changequantity.h"

changeQuantity::changeQuantity(Article* a, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeQuantity)
{
    ui->setupUi(this);
    article = a;
    ui->txtQuantity->setText(QString::number(a->getQuantity()));
    ui->txtQuantity->setFocus();
}

changeQuantity::~changeQuantity()
{
    delete ui;
}

void changeQuantity::on_btnCancel_clicked()
{
    this->close();
    delete this;
}

void changeQuantity::on_btnOk_clicked()
{
    QString sQuantity = ui->txtQuantity->text().replace(',','.');
    float quantity = sQuantity.toFloat();
    if (sQuantity != "")
        article->setQuantity(quantity);

    this->close();
    delete this;
}
