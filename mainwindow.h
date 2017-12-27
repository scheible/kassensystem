#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QString>
#include <QKeyEvent>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFileInfo>
#include "sale.h"
#include "articlefactory.h"
#include "categorybuttons.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ArticleFactory* artFact;
    QList<Sale*> salesQueue;
    int currentSaleIndex = 0;
    CategoryButtons* catButtons;
    QSqlDatabase db;

    void unhookSale(Sale*);
    void hookSale(Sale*);
    void addArticle(Article*,float);

public:
    Sale* sale;

public slots:
    void deleteArticle();
    void addArticlePlu(int,float);
    void addArticleEAN(QString,float=1);
    void newSale();
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void showMessage(QString msg);
    void on_btnAddArticle_clicked();
    void on_btnDeleteArticle_clicked();
    void updateTotal(float total);
    void updateTime(QDateTime datetime);

    void on_txtSearch_returnPressed();
    void on_btnCategoryGoToRoot_clicked();
    void on_btnSaleBack_clicked();
    void on_btnSaleNext_clicked();

    void on_btnPay_clicked();
    void on_btnJumpNewestSale_clicked();
    void on_btnQuantityDec_clicked();
    void on_btnQuantityInc_clicked();
    void on_btnQuantityEnter_clicked();
    void on_btnCustomer_clicked();
    void on_btnDeleteAll_clicked();
    void on_btnCalculator_clicked();
};

#endif // MAINWINDOW_H