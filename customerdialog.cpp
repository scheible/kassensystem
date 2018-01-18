#include "customerdialog.h"
#include "ui_customerdialog.h"
#include "sale.h"

CustomerDialog::CustomerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerDialog)
{
    ui->setupUi(this);
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
}
