#include "customerdialog.h"
#include "ui_customerdialog.h"
#include "sale.h"
#include <QDebug>

CustomerDialog::CustomerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerDialog)
{
    ui->setupUi(this);
    ui->treeView->setColumnWidth(1,1000);
    this->updateBack(0);
    this->updateTotal(0);
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
}

void CustomerDialog::updateTotal(float total) {
    ui->lblToPay->setText(QString::number(total,'f',2) + " €");
}

void CustomerDialog::updateBack(float change) {
    ui->lblChange->setText(QString::number(change,'f',2) + " €");
}
