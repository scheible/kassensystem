#ifndef SALEFACTORY_H
#define SALEFACTORY_H

#include <QObject>

#include <sale.h>

class SaleFactory : public QObject
{
    Q_OBJECT

    Sale *loadSingleSaleFromDatabase(int saleId);
    void checkDeposit(Article *art);

public:
    explicit SaleFactory(QObject *parent = nullptr);
    int loadOldSalesFromDatabase(int n, QList<Sale*>*);


signals:

public slots:
};

#endif // SALEFACTORY_H
