#include "freearticle.h"
#include "ui_freearticle.h"
#include "mainwindow.h"
#include "article.h"

FreeArticle::FreeArticle(MainWindow* w, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FreeArticle)
{
    ui->setupUi(this);
    mWindow = w;
}

FreeArticle::~FreeArticle()
{
    delete ui;
}

void FreeArticle::on_btnOk_clicked()
{
    float quantity = 0;
    float price = 0;
    QString name = "Freiartikel";

    if (ui->txtName->text() != "") {
        name = ui->txtName->text();
    }

    quantity = ui->txtQuantity->text().toInt();
    price = ui->txtPrice->text().replace(",",".").toFloat();
    if (quantity == 0)
        quantity = 1;

    Article *art = new Article();
    art->setQuantity(quantity);
    art->setSinglePriceNormal(price);
    art->setName(name);
    art->setPLU(0);

    mWindow->sale->addArticle(art);
    this->close();
    delete this;
}

void FreeArticle::on_btnCancel_clicked()
{
    this->close();
    delete this;
}
