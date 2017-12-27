#ifndef ARTICLEFACTORY_H
#define ARTICLEFACTORY_H

#include "article.h"
#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

class ArticleFactory : public QObject
{
    Q_OBJECT

private:
    QString databaseFileName;
    QSqlDatabase db;
    bool accessible = false;

    Article* loadDataFromQuery(QSqlQuery query);

public:
    explicit ArticleFactory(QString databaseFileName, QObject *parent = 0);

    QString getDatabaseFileName();
    bool isDatabaseAccessible();
    Article* newPLUArticleFromDb(int plu);
    Article* newIDArticleFromDb(int id);
    Article* newBarcodeArticleFromDb(QString barcode);
    bool isTakeBackArticleID(int id);
    bool isTakeBackArticlePLU(int plu);
    bool isTakeBackArticleBarcode(QString barcode);

signals:

public slots:
};

#endif // ARTICLEFACTORY_H
