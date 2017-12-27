#ifndef ARTICLE_H
#define ARTICLE_H

#include <QObject>
#include <QStandardItem>
#include <QDate>

class Article : public QObject
{
    Q_OBJECT

private:
    int id = -1;
    int plu = -1;
    QString name = "";
    float quantity = 1;
    int despositPLU = -1;
    float priceNormal = 0;
    float priceOffer = 0;
    QDate offerDate;
    //bool takeBackFlag = false;
    float multipl = 1;
    bool isDepositFlag = false;
    QString barcode;
    QString category;
    QString unit;
    bool aboutToDelete = false;
    bool locked = false;

    void updatePricesLabels();

public:
    explicit Article(QObject *parent = 0);
    ~Article();

    void setID(int id);
    void setPLU(int plu);
    void setName(QString name);
    void setUnit(QString unit);
    //void setQuantity(float quantity);
    void setDepositPLU(int plu);
    void setSinglePriceNormal(float price);
    void setSinglePriceOffer(float price);
    void setOfferDate(QDate date);
    void setCategory(QString category);
    //void markAsTakeBack(bool isNegative);
    void setMultipl(float multipl);
    void setBarcode(QString barcode);
    void markAsDeposit();
    void addQuantity(float quantity);
    void lock();
    void unlock();

    int getID();
    int getPLU();
    QString getName();
    QString getBarName();
    float getQuantity();
    int getDepositPLU();
    float getSinglePrice();
    float getTotalPrice();
    float getNormalPrice();
    float getOfferPrice();
    QString getUnit();
    bool hasDeposit();
    bool isDeposit();
    bool isTakeBack();
    float getMultipl();
    bool isOffer();
    QDate getOfferEndDate();
    QString getCategory();
    QString getBarcode();

    bool isAboutToDelete();

signals:
    void quantityChanged(float quantity);
    void totalPriceChanged();
    void dueToDelete();

public slots:
    void setQuantity(float quantity);
    void deleteThis();
};

#endif // ARTICLE_H




