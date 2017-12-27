#include "searchcustomer.h"
#include "ui_searchcustomer.h"
#include "customersearchtablemodel.h"

#include <QModelIndexList>
#include <newcustomer.h>

SearchCustomer::SearchCustomer(Sale *sale, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchCustomer)
{
    ui->setupUi(this);
    model = new CustomerSearchTableModel();
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();
    //ui->tableView->horizontalHeader()->hide();
    ui->tableView->setShowGrid(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    this->sale = sale;

}

SearchCustomer::~SearchCustomer()
{
    delete ui;
    delete model;
}

void SearchCustomer::on_txtSearch_returnPressed()
{
    static_cast<CustomerSearchTableModel*>(ui->tableView->model())->search(ui->txtSearch->text());
}

void SearchCustomer::on_btnOk_clicked()
{
    QModelIndexList list = ui->tableView->selectionModel()->selectedRows();
    if (list.length() > 0) {
        Customer *c = model->getCustomer(list.at(0).row());
        sale->setCustomer(c);
        delete this;
    }
}

void SearchCustomer::on_btnCancel_clicked()
{
    this->hide();
    delete this;
}

void SearchCustomer::on_btnNew_clicked()
{
    NewCustomer *dialog = new NewCustomer(this);

    dialog->show();
}
void SearchCustomer::search(QString searchTerm) {
    ui->txtSearch->setText(searchTerm);
    on_txtSearch_returnPressed();
}
