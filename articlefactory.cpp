#include "articlefactory.h"
#include <QDebug>

ArticleFactory::ArticleFactory(QString databaseFileName, QObject *parent) : QObject(parent)
{
}

QString ArticleFactory::getDatabaseFileName() {
    return this->databaseFileName;
}
bool ArticleFactory::isDatabaseAccessible() {
    return this->accessible;
}
Article* ArticleFactory::newPLUArticleFromDb(int plu) {
    QSqlQuery query;
    QString command = "SELECT a.id,\
                    a.plu, \
                    a.name, \
                    a.depositId, \
                    a.price, \
                    a.offerPrice, \
                    a.offerEnd, \
                    u.name, \
                    a.ean, \
                    a.multipl \
                FROM \
                    article as a \
                LEFT JOIN unit as u ON a.unitId = u.id \
                WHERE \
                    a.plu = "+QString::number(plu)+";";


    if (query.exec(command))
    {
        return this->loadDataFromQuery(query);
    } else {
        return 0;
    }
}
Article* ArticleFactory::newIDArticleFromDb(int id) {
    QSqlQuery query;
    QString command = " SELECT a.id,\
                            a.plu, \
                            a.name, \
                            a.depositId, \
                            a.price, \
                            a.offerPrice, \
                            a.offerEnd, \
                            u.name, \
                            a.ean, \
                            a.multipl \
                        FROM \
                            article as a \
                        LEFT JOIN unit as u ON a.unitId = u.id \
                        WHERE \
                            a.id = "+QString::number(id)+";";

    if (query.exec(command))
    {
        return this->loadDataFromQuery(query);
    } else {
        return false;
    }
}
Article* ArticleFactory::newBarcodeArticleFromDb(QString barcode) {
    QSqlQuery query;
    QString command = " SELECT a.id,\
                            a.plu, \
                            a.name, \
                            a.depositId, \
                            a.price, \
                            a.offerPrice, \
                            a.offerEnd, \
                            u.name, \
                            a.ean, \
                            a.multipl \
                        FROM \
                            article as a \
                        LEFT JOIN unit as u ON a.unitId = u.id \
                        WHERE \
                            a.ean = "+barcode+";";

    if (query.exec(command))
    {
        return this->loadDataFromQuery(query);
    } else {
        return false;
    }
}

Article* ArticleFactory::loadDataFromQuery(QSqlQuery query) {

    Article* art = 0;

    if (query.next()) {
        art = new Article();

        //Fill the fields of the object from the database
        art->setID(                query.value(0).toInt()      );
        art->setPLU(               query.value(1).toInt()      );
        art->setName(              query.value(2).toString()   );
        art->setDepositPLU(        query.value(3).toInt()      );
        art->setSinglePriceNormal( query.value(4).toFloat()    );
        art->setSinglePriceOffer(  query.value(5).toFloat()    );
        art->setOfferDate(         query.value(6).toDate()     );
        art->setBarcode(           query.value(7).toString()   );
        art->setQuantity(1);    //set Quantity to 1 as a standard
        art->setMultipl(           query.value(9).toFloat()    );
    }

    return art;
}

bool ArticleFactory::isTakeBackArticleID(int id) {
    QSqlQuery query;

    if (query.exec("SELECT multipl FROM article WHERE id = " + QString::number(id) + ";")) {
        if(query.next()) {
            if (query.value(0).toInt() < 0) {
                return true;
            }
        }
    }
    return false;
}
bool ArticleFactory::isTakeBackArticlePLU(int plu) {
    QSqlQuery query;

    if (query.exec("SELECT multipl FROM article WHERE plu = " + QString::number(plu) + ";")) {
        if(query.next()) {
            if (query.value(0).toInt() < 0) {
                return true;
            }
        }
    }
    return false;
}
bool ArticleFactory::isTakeBackArticleBarcode(QString barcode) {
    return false;
}
