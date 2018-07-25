#include "reporting.h"
#include "ui_reporting.h"

Reporting::Reporting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reporting)
{
    QDate d;

    ui->setupUi(this);
    this->ui->dateDay->setDate(d.currentDate());

    this->getDayTurnover(d.currentDate());
}

Reporting::~Reporting()
{
    delete ui;
}

int Reporting::getDayArticles(QDate date) {
    // Get the number of articles that have been sold that day
    // It counts the number of items of a sale times the quantity
    // and sums it up for all sales

    QSqlQuery query;
    QString command = " SELECT\
                            sum(sale_article.quantity)\
                        FROM\
                            sale, sale_article\
                        WHERE\
                            sale.id = sale_article.sale_id AND \
                            date(sale.endTime) = date(\'" + date.toString("yyyy-MM-dd") + "\');";

    if (query.exec(command))
    {
        float quantity;
        if (query.next()) {
            quantity = query.value(0).toInt();
        } else {
            qDebug() << "No data";
        }

        return quantity;
    } else {
        qDebug() << "Could not create reporting " << command;
    }
}

float Reporting::getDayTurnover(QDate date) {
    // This function gets the sum of all sales of the given date
    // This is a single number

    QSqlQuery query;
    QString command = " SELECT\
                            sum(sale_article.price * sale_article.quantity) as \"total\"\
                        FROM\
                            sale, sale_article\
                        WHERE\
                            sale.id = sale_article.sale_id AND\
                            date(sale.endTime) = date(\'" + date.toString("yyyy-MM-dd") + "\');";

    if (query.exec(command))
    {
        float sum;
        if (query.next()) {
            sum = query.value(0).toFloat();
        } else {
            qDebug() << "No data";
        }

        return sum;
    } else {
        qDebug() << "Could not create reporting " << command;
    }
}

void Reporting::on_chbSpecifyDates_stateChanged(int arg1)
{
    this->ui->dateStart->setEnabled(arg1);
    this->ui->dateEnd->setEnabled(arg1);
}

void Reporting::on_pushButton_clicked()
{
    this->close();
    delete this;
}

void Reporting::on_dateDay_userDateChanged(const QDate &date)
{
    float turnover = this->getDayTurnover(date);
    int quantity = this->getDayArticles(date);
    this->ui->lblTurnover->setText(QString::number(turnover) + " €");
    this->ui->lblArticle->setText(QString::number(quantity));
}

void Reporting::on_btnSaveAs_clicked()
{
    // In the selection box the type of report can be selected
    // there is a function for every type
    int index = this->ui->cbxReportSelection->currentIndex();

    if (index == 0) {
        this->turnoverPerDayToExcel(this->ui->chbSpecifyDates->checkState(), this->ui->dateStart->date(), this->ui->dateEnd->date());
    } else if (index == 1) {
        this->turnoverPerArticle(this->ui->chbSpecifyDates->checkState(), this->ui->dateStart->date(), this->ui->dateEnd->date());;
    } else if (index == 2) {
        this->turnoverPerSale(this->ui->chbSpecifyDates->checkState(), this->ui->dateStart->date(), this->ui->dateEnd->date());;
    }
}

void Reporting::turnoverPerDayToExcel(bool limitDate, QDate fromDate, QDate toDate) {
    //Query the turnover summarized per day and save it to an excel file

    QSqlQuery query;
    QString command1 = " SELECT\
                            DATE(sale.endTime),\
                            SUM(sale_article.quantity),\
                            SUM(sale_article.price * sale_article.quantity)\
                        FROM\
                            sale, sale_article\
                        WHERE\
                            sale.id = sale_article.sale_id";
    QString searchString = " AND\
                            DATE(sale.endTime) >= DATE(\'" + fromDate.toString("yyyy-MM-dd") + "\') AND\
                            DATE(sale.endTime) <= DATE(\'" + toDate.toString("yyyy-MM-dd") + "\')";
    QString command2 = " GROUP BY\
                            DATE(sale.endTime);";

    QString command;
    if (limitDate) {
            qDebug() << "Limit date";
            command = command1 + searchString + command2;
            qDebug() << command;
    } else {
        command = command1 + command2;
    }

    QXlsx::Document xlsx;

    if (query.exec(command))
    {
        xlsx.write("A1", "Datum");
        xlsx.write("B1", "Artikel");
        xlsx.write("C1", "Gesamt EUR");

        int i=2;
        while (query.next()) {
            QDate date = query.value(0).toDate();
            float quantity = query.value(1).toFloat();
            float sum = query.value(2).toFloat();

            xlsx.write("A" + QString::number(i), date);
            xlsx.write("B" + QString::number(i), quantity);
            xlsx.write("C" + QString::number(i), sum);
            i++;
        }
        QFileDialog diag;
        QString fileName = diag.getSaveFileName(0, "Verkaufsdaten exportieren","verkaufsdaten.xlsx","*.xlsx");

        if (!(fileName.right(5).toUpper() == ".XLSX"))
            fileName = fileName + ".xlsx";

        xlsx.saveAs(fileName);

    } else {
        qDebug() << "Could not create report " << command;
    }
}

void Reporting::turnoverPerArticle(bool limitDate, QDate fromDate, QDate toDate) {
    //Query the turnover summarized per article

    QSqlQuery query;
    QString command1 = "SELECT\
                            article.plu,\
                            article.name,\
                            SUM(quantity)\
                        FROM article \
                            LEFT JOIN sale_article ON article.id = sale_article.article_id\
                            LEFT JOIN sale ON sale_article.sale_id = sale.id";


    QString searchString = " WHERE\
                                DATE(sale.endTime) >= DATE(\'" + fromDate.toString("yyyy-MM-dd") + "\') AND\
                                DATE(sale.endTime) <= DATE(\'" + toDate.toString("yyyy-MM-dd") + "\')";
    QString command2 = " GROUP BY article.plu;";

    QString command;
    if (limitDate) {
            qDebug() << "Limit date";
            command = command1 + searchString + command2;
            qDebug() << command;
    } else {
        command = command1 + command2;
    }

    QXlsx::Document xlsx;

    if (query.exec(command))
    {
        xlsx.write("A1", "PLU");
        xlsx.write("B1", "Artikelbezeichnung");
        xlsx.write("C1", "Menge");

        int i=2;
        while (query.next()) {
            int plu = query.value(0).toInt();
            QString name = query.value(1).toString();
            float quantity = query.value(2).toFloat();

            xlsx.write("A" + QString::number(i), plu);
            xlsx.write("B" + QString::number(i), name);
            xlsx.write("C" + QString::number(i), quantity);
            i++;
        }
        QFileDialog diag;
        QString fileName = diag.getSaveFileName(0, "Verkaufsdaten exportieren","verkaufsdaten.xlsx","*.xlsx");

        if (!(fileName.right(5).toUpper() == ".XLSX"))
            fileName = fileName + ".xlsx";

        xlsx.saveAs(fileName);

    } else {
        qDebug() << "Could not create report " << command;
    }
}

void Reporting::turnoverPerSale(bool limitDate, QDate fromDate, QDate toDate) {
    QSqlQuery query;
    QString command1 = " SELECT\
                            count(sale.id)  as \"num_articles\",\
                            DATE(sale.endTime) as date,\
                            TIME(sale.endTime) as time,\
                            sum(sale_article.price * sale_article.quantity) as \"total\"\
                        FROM\
                            sale, sale_article\
                        WHERE\
                            sale.id = sale_article.sale_id";
    QString searchString = " AND\
                            DATE(sale.endTime) >= DATE(\'" + fromDate.toString("yyyy-MM-dd") + "\') AND\
                            DATE(sale.endTime) <= DATE(\'" + toDate.toString("yyyy-MM-dd") + "\')";
    QString command2 = " GROUP BY\
                            sale.id;";

    QString command;
    if (limitDate) {
            qDebug() << "Limit date";
            command = command1 + searchString + command2;
            qDebug() << command;
    } else {
        command = command1 + command2;
    }

    QXlsx::Document xlsx;

    if (query.exec(command))
    {
        xlsx.write("A1", "Datum");
        xlsx.write("B1", "Uhrzeit");
        xlsx.write("C1", "Artikelanzahl");
        xlsx.write("D1", "Gesamt EUR");

        int i=2;
        while (query.next()) {
            int count = query.value(0).toInt();
            QDate date = query.value(1).toDate();
            QTime time = query.value(2).toTime();
            float sum = query.value(3).toFloat();

            xlsx.write("A" + QString::number(i), date);
            xlsx.write("B" + QString::number(i), time);
            xlsx.write("C" + QString::number(i), count);
            xlsx.write("D" + QString::number(i), sum);
            i++;
        }
        QFileDialog diag;
        QString fileName = diag.getSaveFileName(0, "Verkaufsdaten exportieren","verkaufsdaten.xlsx","*.xlsx");

        if (!(fileName.right(5).toUpper() == ".XLSX"))
            fileName = fileName + ".xlsx";

        xlsx.saveAs(fileName);

    } else {
        qDebug() << "Could not create report " << command;
    }
}

void Reporting::on_Reporting_destroyed()
{
    qDebug() << "closing reporting";
}
