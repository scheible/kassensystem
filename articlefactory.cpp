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
                    a.multipl, \
                    a.color \
                FROM \
                    article as a \
                LEFT JOIN unit as u ON a.unitId = u.id \
                WHERE \
                    a.plu = "+QString::number(plu)+";";


    if (query.exec(command))
    {
        return this->loadDataFromQuery(query);
    } else {
        qDebug() << "couldn't load article by PLU (" << command << ")";
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
                            a.multipl, \
                            a.color \
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
                            a.multipl, \
                            a.color \
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
    // It is possible that the query returns more than one article
    // TODO: In this case show a window that asks for the article

    QList<Article *> artList;
    Article* art = 0;

    while (query.next()) {
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
        art->setColor(             query.value(10).toString()  );

        qDebug() << "found a new article!";
        artList.append(art);
    }

    // There were more than 1 article found in the database
    // -> Show a dialog from which the user can select the article
    if (artList.length() > 1) {
        ArticleSelectorDialog *sDialog = new ArticleSelectorDialog();
        sDialog->setArticles(&artList);
        if (sDialog->exec() > 0) {
            art = sDialog->getSelectedArticle();
            delete sDialog;
        } else {
            art = nullptr;
        }
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
