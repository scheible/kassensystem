#ifndef SALE_H
#define SALE_H

#include "article.h"
#include "customer.h"
#include <QObject>
#include <QDateTime>
#include <QStandardItemModel>
#include <QList>
#include <QItemSelection>

#include <QAbstractItemModel>

class Sale : public QAbstractItemModel
{
    Q_OBJECT

private:
    QDateTime timestampStart;
    QDateTime timestampPay;
    QList<Article*> articles;
    float givenMoney = 0;
    QList<bool> color;
    int id = -1;
    bool locked = false;
    Customer* customer;

    void refreshColorList();
    //void dbUpdateStartTime();
    void dbInit();
    void dbUpdatePayTime();
    void dbUpdateCustomer();
    void dbUpdateGivenMoney();
    void dbSaveArticles();


public:
    explicit Sale(QObject *parent = 0);
    ~Sale();

    Article* getArticlePLU(int plu);
    Article* getArticleID(int id);
    Article* getArticleBarcode(QString barcode);
    Article* getArticleIndex(int index);

    void setCustomer(Customer* c);
    void clear();
    float getTotal();
    QDateTime getTimestampStart();
    QDateTime getTimestampPay();
    bool isLocked();
    float pay(float givenMoney);
    float getGivenMoney();

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool hasChildren(const QModelIndex & parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    void totalChanged(float total);
    void newMessage(QString msg);
    void articleDeleted();
    void startTimeIsUpdated(QDateTime);

public slots:
    void addArticle(Article* art);
    void updateTotal();
    void deleteArticle();
    void refresh();


};

#endif // SALE_H
