#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>
#include <QTreeView>
#include <QMainWindow>
#include "sale.h"

namespace Ui {
class CustomerDialog;
}

class CustomerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerDialog(QMainWindow *m, QWidget *parent = 0);
    ~CustomerDialog();
    QTreeView* getTreeView();
    void setModel(Sale*);

    bool focusInEvent(QEvent *event);

private:
    Ui::CustomerDialog *ui;
    QMainWindow *m;


public slots:
    void updateTotal(float total);
    void updateBack(float change);

};

#endif // CUSTOMERDIALOG_H
