#include "newcustomer.h"
#include "ui_newcustomer.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

NewCustomer::NewCustomer(SearchCustomer *dialog, QWidget *parent) :
    QDialog(parent)
{
    ui = new Ui::NewCustomer;
    ui->setupUi(this);
    searchDialog = dialog;

    ui->cbxTitle->addItem("Herr");
    ui->cbxTitle->addItem("Frau");
}

NewCustomer::~NewCustomer()
{
    delete ui;
}

void NewCustomer::on_btnOk_clicked()
{
    QSqlQuery query;
    QString id;

    if (query.exec("INSERT INTO customer (firstname,lastname,title,street,number,zip,city,phone,email) VALUES ('" +
               ui->txtFirstName->text() + "','" +
               ui->txtLastName->text() + "','" +
               ui->cbxTitle->currentText() + "','" +
               ui->txtStreet->text() + "','" +
               ui->txtNumber->text() + "','" +
               ui->txtZip->text() + "','" +
               ui->txtCity->text() + "','" +
               ui->txtPhone->text() + "','" +
               ui->txtEmail->text() + "');")) {
        query.exec("SELECT last_insert_rowid()");
        if (query.next()) {
            id = query.value(0).toString();
        }
    } else {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError().text();
        //display error message
    }

    if (searchDialog != 0) {
       searchDialog->search(id);
    }
    delete this;
}

void NewCustomer::on_btnCancel_clicked()
{
    this->hide();
    delete this;
}
