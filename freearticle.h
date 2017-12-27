#ifndef FREEARTICLE_H
#define FREEARTICLE_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class FreeArticle;
}

class FreeArticle : public QDialog
{
    Q_OBJECT

private:
    MainWindow* mWindow;

public:
    explicit FreeArticle(MainWindow*, QWidget *parent = 0);
    ~FreeArticle();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::FreeArticle *ui;
};

#endif // FREEARTICLE_H
