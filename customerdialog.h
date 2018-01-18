#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>
#include <QTreeView>
#include "sale.h"

namespace Ui {
class CustomerDialog;
}

class CustomerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerDialog(QWidget *parent = 0);
    ~CustomerDialog();
    QTreeView* getTreeView();
    void setModel(Sale*);



private:
    Ui::CustomerDialog *ui;

};

#endif // CUSTOMERDIALOG_H
