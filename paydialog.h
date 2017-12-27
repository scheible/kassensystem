#ifndef PAYDIALOG_H
#define PAYDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QPalette>

namespace Ui {
class PayDialog;
}

class PayDialog : public QDialog
{
    Q_OBJECT

private:
    float givenMoney = 0;
    float change = 0;

    QPalette red;
    QPalette black;

public:
    explicit PayDialog(MainWindow *w,QWidget *parent = 0);
    ~PayDialog();

private slots:
    void on_btnCancel_clicked();

    void on_btnOkWithBon_clicked();

    void on_btnOkWithoutBon_clicked();

    void on_txtGiven_returnPressed();

    void on_txtGiven_editingFinished();

    void on_txtGiven_textChanged(const QString &arg1);

    void on_btnNew_clicked();

    void on_txtBack_textChanged(const QString &arg1);

private:
    Ui::PayDialog *ui;
    MainWindow *mWindow;
};

#endif // PAYDIALOG_H
