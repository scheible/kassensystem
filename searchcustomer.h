#ifndef SEARCHCUSTOMER_H
#define SEARCHCUSTOMER_H

#include <QDialog>
#include "sale.h"
#include "customersearchtablemodel.h"

namespace Ui {
class SearchCustomer;
}

class SearchCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit SearchCustomer(Sale *sale, QWidget *parent = 0);
    ~SearchCustomer();
    void search(QString);

private slots:
    void on_txtSearch_returnPressed();

    //void on_pushButton_3_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_btnNew_clicked();

private:
    Ui::SearchCustomer *ui;
    Sale *sale;
    CustomerSearchTableModel *model;

};

#endif // SEARCHCUSTOMER_H
