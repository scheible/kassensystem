#ifndef NEWCUSTOMER_H
#define NEWCUSTOMER_H

#include <QDialog>
#include "searchcustomer.h"

namespace Ui {
class NewCustomer;
}

class NewCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit NewCustomer(SearchCustomer *dialog = 0, QWidget *parent = 0);
    ~NewCustomer();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::NewCustomer *ui;
    SearchCustomer *searchDialog;

};

#endif // NEWCUSTOMER_H
