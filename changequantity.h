#ifndef CHANGEQUANTITY_H
#define CHANGEQUANTITY_H

#include <QDialog>
#include "article.h"

namespace Ui {
class changeQuantity;
}

class changeQuantity : public QDialog
{
    Q_OBJECT

public:
    explicit changeQuantity(Article *a, QWidget *parent = 0);
    ~changeQuantity();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Article *article;
    Ui::changeQuantity *ui;
};

#endif // CHANGEQUANTITY_H
