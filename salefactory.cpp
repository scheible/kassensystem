#include "article.h"
#include "sale.h"
#include "salefactory.h"

#include <QSqlQuery>
#include <QDebug>

SaleFactory::SaleFactory(QObject *parent) : QObject(parent)
{

}

int SaleFactory::loadOldSalesFromDatabase(int n, QList<Sale*> *salesQueue) {
    QSqlQuery query;
    QString command = "SELECT * FROM sale ORDER BY startTime DESC LIMIT :limit";
    query.prepare(command);
    query.bindValue(":limit", n);
    int id;
    Sale *sale;
    int counter=0;

    if (query.exec())
    {
        while (query.next()) {
            counter++;
            id = query.value("id").toInt();
            sale = this->loadSingleSaleFromDatabase(id);
            sale->setStartTime(query.value("startTime").toDateTime());
            salesQueue->append(sale);
        }
        return counter;
    } else {
        qDebug() << "couldn't load old sales (" << command << ")";
        return 0;
    }
}


Sale *SaleFactory::loadSingleSaleFromDatabase(int saleId) {
    QString command = "SELECT plu, article_name as name, quantity, sale_article.price as price \
                       FROM sale_article LEFT JOIN article ON sale_article.article_id = article.id \
                       WHERE sale_id = :saleId ORDER BY position ASC";
    QSqlQuery query;
    query.prepare(command);
    query.bindValue(":saleId", saleId);
    Sale *sale;

    int plu;
    QString name;
    float quantity;
    float price;
    Article *art;

    if (query.exec()) {
        sale = new Sale();
        sale->setId(saleId);

        while (query.next()) {
            plu = query.value("plu").toInt();
            name = query.value("name").toString();
            quantity = query.value("quantity").toFloat();
            price = query.value("price").toFloat();

            art = new Article();
            art->setPLU(plu);
            art->setName(name);
            art->setQuantity(quantity);
            art->setSinglePriceNormal(price);

            this->checkDeposit(art);

            sale->addArticle(art);
            art->lock();
        }
        sale->setLocked();
        return sale;
    } else {
        qDebug() << "could not load sale object(" << command << ")";
        return NULL;
    }
}

void SaleFactory::checkDeposit(Article *art) {
    QString name = art->getName();

    if (name.trimmed().startsWith("Pfand")) {
        name.remove(0,10);
        name.remove(name.length()-1,1);
        art->setName(name);
        art->markAsDeposit();
    }

    if (name.trimmed().startsWith("Zurück")) {
        art->setMultipl(-1);
    }
}

