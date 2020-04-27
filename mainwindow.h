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
#include "customerdialog.h"

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
    QString databaseFileName;
    bool showWarningOnArticleNotFound = true;
    int nRestoreOldSales = 0;
    void unhookSale(Sale*);
    void hookSale(Sale*);
    void addArticle(Article*,float);
    int customerDialogMode;
    void loadOldSales();

public:
    Sale* sale;
    CustomerDialog *customerDialog;

public slots:
    void deleteArticle();
    void addArticlePlu(int,float);
    void addArticleEAN(QString,float=1);
    void newSale();
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent *);

private slots:
    void showMessage(QString msg);
    void on_btnAddArticle_clicked();
    void on_btnDeleteArticle_clicked();
    void updateTotal(float total);
    void updateTime(QDateTime datetime);
    void updateArticleCount(float count);
    void updateTakebackCount(float count);

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
    void on_btnReporting_clicked();
    void on_btnToggleCustomerDisplay_toggled(bool checked);
};

#endif // MAINWINDOW_H
