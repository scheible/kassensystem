#include "article.h"
#include <QtGlobal>
#include <QDebug>

Article::Article(QObject *parent) : QObject(parent)
{
}
Article::~Article() {
    //Delete the items
    aboutToDelete = true;
    emit dueToDelete();
}

void Article::updatePricesLabels() {
    emit totalPriceChanged();
}

void Article::setID(int id) {
    if (!locked)
        this->id = id;
}
void Article::setPLU(int plu) {
    if (!locked)
        this->plu = plu;
}
void Article::setName(QString name) {
    if (!locked)
        this->name = name;
}
void Article::setUnit(QString unit) {
    if (!locked)
        this->unit = unit;
}
void Article::setDepositPLU(int plu) {
    if (!locked)
        this->despositPLU = plu;
}
void Article::setSinglePriceNormal(float price) {
    if (!locked) {
        this->priceNormal = price;
        this->updatePricesLabels();
    }
}
void Article::setSinglePriceOffer(float price) {
    if (!locked) {
        this->priceOffer = price;
        this->updatePricesLabels();
    }
}
void Article::setOfferDate(QDate date) {
    if (!locked) {
        this->offerDate = date;
        this->updatePricesLabels();
    }
}
void Article::setCategory(QString category) {
    if (!locked)
        this->category = category;
}
void Article::setMultipl(float multipl) {
    this->multipl = multipl;
}

void Article::setBarcode(QString barcode) {
    if (!locked) {
        this->barcode = barcode;
    }
}
void Article::markAsDeposit() {
    if (!locked)
        this->isDepositFlag = true;
}
void Article::addQuantity(float quantity) {
    if (!locked) {
        this->setQuantity(this->quantity + quantity);
        emit this->totalPriceChanged();
        emit this->quantityChanged(this->quantity);
    }
}

int Article::getID() {
    return this->id;
}
int Article::getPLU() {
    return this->plu;
}
QString Article::getName() {
    if (this->isDeposit()) {
        return "   Pfand (" + this->name + ")";
    } else {
        if ((this->isTakeBack() && this->getQuantity() > 0) || (!this->isTakeBack() && this->getQuantity() < 0)) {
            return "Zurück: " + this->name;
        } else {
            if (this->isOffer()) {
                return this->name + " ***Angebotspreis***";
            } else {
                return this->name;
            }
        }
    }
}
QString Article::getBarName() {
    return this->name;
}

QString Article::getUnit() {
    return this->unit;
}
float Article::getQuantity() {
    return this->quantity;
}
int Article::getDepositPLU() {
    return this->despositPLU;
}
float Article::getSinglePrice() {
    if (this->isOffer()) {
        return this->getOfferPrice();
    } else {
        return this->getNormalPrice();
    }
}
float Article::getTotalPrice() {
    //Round to two decimals
    float totalPrice = this->getSinglePrice()*this->quantity*100;
    totalPrice = (float)qRound(totalPrice) / 100;

    return totalPrice;
}
float Article::getNormalPrice() {
    if (this->isTakeBack()){
        return -this->priceNormal;
    } else {
        return this->priceNormal;
    }
}
float Article::getOfferPrice() {
    if (this->isTakeBack()) {
        return -this->priceOffer;
    } else {
        return this->priceOffer;
    }
}
bool Article::hasDeposit() {
    if (this->despositPLU == 0) {
        return false;
    } else {
        return true;
    }
}
bool Article::isDeposit() {
    return this->isDepositFlag;
}
bool Article::isOffer() {
    //check if offer date is higher than today
    if (this->offerDate >= QDate::currentDate() && this->priceOffer != 0) {
        return true;
    } else {
        return false;
    }
}
QDate Article::getOfferEndDate() {
    return this->offerDate;
}
QString Article::getCategory() {
    return this->category;
}
bool Article::isTakeBack() {
    return (this->multipl < 0 && !this->isDeposit());
}
QString Article::getBarcode() {
    return this->barcode;
}

//public slots:
void Article::setQuantity(float quantity) {
    if (!locked){
        this->quantity = quantity;
        this->updatePricesLabels();
        emit quantityChanged(this->quantity);
        emit totalPriceChanged();
    }
}
void Article::deleteThis() {
    //this article should be deleted
    if (!locked)
        delete this;
}
bool Article::isAboutToDelete() {
    return this->aboutToDelete;
}
void Article::lock() {
    locked = true;
}
void Article::unlock() {
    locked = false;
}
float Article::getMultipl() {
    return this->multipl;
}
