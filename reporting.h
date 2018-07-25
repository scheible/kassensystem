#ifndef REPORTING_H
#define REPORTING_H

#include <QDialog>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "xlsxdocument.h"
#include <QFileDialog>

namespace Ui {
class Reporting;
}

class Reporting : public QDialog
{
    Q_OBJECT

public:
    explicit Reporting(QWidget *parent = 0);
    ~Reporting();

private slots:
    void on_chbSpecifyDates_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_dateDay_userDateChanged(const QDate &date);

    void on_btnSaveAs_clicked();

    void on_Reporting_destroyed();

private:
    Ui::Reporting *ui;
    float getDayTurnover(QDate);
    int getDayArticles(QDate);
    void turnoverPerDayToExcel(bool limitDate, QDate fromDate, QDate toDate);
    void turnoverPerArticle(bool limitDate, QDate fromDate, QDate toDate);
    void turnoverPerSale(bool limitDate, QDate fromDate, QDate toDate);
};

#endif // REPORTING_H
