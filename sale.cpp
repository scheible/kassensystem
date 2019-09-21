#include "sale.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

Sale::Sale(QObject *parent) : QAbstractItemModel(parent)
{
}

Sale::~Sale() {
    //delete all articles
    for (int i=0;i < articles.count(); i++) {
        articles.at(i)->unlock();
        delete articles.at(i);
    }
}

Article* Sale::getArticlePLU(int plu) {
    for (int i = 0;i < articles.count(); i++) {
        if (articles.at(i)->getPLU() == plu && !articles.at(i)->isDeposit()) {
            return articles.at(i);
        }
    }
    return 0;
}
Article* Sale::getArticleID(int id) {
    for (int i = 0;i < articles.count(); i++) {
        if (articles.at(i)->getID() == id && !articles.at(i)->isDeposit()) {
            return articles.at(i);
        }
    }
    return 0;
}
Article* Sale::getArticleBarcode(QString barcode) {
    for (int i = 0;i < articles.count(); i++) {
        if (articles.at(i)->getBarcode() == barcode && !articles.at(i)->isDeposit()) {
            return articles.at(i);
        }
    }
    return 0;
}
Article* Sale::getArticleIndex(int index) {
    return articles.at(index);
}

void Sale::setCustomer(Customer *c) {
    if (c != 0){
        customer = c;
        dbUpdateCustomer();
        newMessage("Kunde: " + customer->getFirstName()+ " " + customer->getLastName());
    }
}

float Sale::getTotal() {
    float sum=0;
    for (int i=0; i < articles.count(); i++) {
        sum += articles.at(i)->getTotalPrice();
    }
    return sum;
}
QDateTime Sale::getTimestampStart() {
    return this->timestampStart;
}
QDateTime Sale::getTimestampPay() {}
float Sale::pay(float givenMoney) {
    if (id > 0 && articles.length() > 0) {
        this->timestampPay = QDateTime::currentDateTime();
        this->givenMoney = givenMoney;

        dbSaveArticles();
        dbUpdatePayTime();
        dbUpdateGivenMoney();
        locked = true;
    }
}
void Sale::dbUpdateCustomer() {
    QSqlQuery query;

    if (customer != 0 && id >= 0) {
        if (query.exec("UPDATE sale SET customer = " + QString::number(customer->getId()) + " WHERE id = " + QString::number(id) + ";")) {
            //Customer updated
        } else {
            //display error message
        }
    }
}

void Sale::dbInit() {
    QSqlQuery query;

    if (id >= 0)
        return;

    if (query.exec("INSERT INTO sale (startTime, staff) VALUES ('" + timestampStart.toString("yyyy-MM-dd hh:mm:ss") + "',0);")) {
        query.exec("SELECT last_insert_rowid()");
        if (query.next()) {
            id = query.value(0).toInt();
        }
    }
}
void Sale::dbUpdateGivenMoney() {
    QSqlQuery query;
    QString sql;

    if (id < 0)
        return;

    sql = "UPDATE sale SET givenMoney = " + QString::number(givenMoney) + " WHERE id = "+QString::number(id)+";";

    if (!query.exec(sql)) {
        qDebug() << "error" << query.lastError().text() << sql;
    }
}
void Sale::dbUpdatePayTime() {
    QSqlQuery query;

    if (id < 0)
        return;

    if (!query.exec("UPDATE sale SET endTime='" + timestampPay.toString("yyyy-MM-dd hh:mm:ss") + "' WHERE id="+QString::number(id)+";")) {
        qDebug() << "error" + query.lastError().text();
    }
}
void Sale::dbSaveArticles() {
    QString sql;
    Article *art;
    QSqlQuery query;

    if (id < 0)
        return;

    for (int i=0; i<articles.length(); i++) {
        art = articles.at(i);
        sql = "INSERT INTO sale_article (sale_id,article_id,article_name,quantity,price,position) VALUES (";
        sql += QString::number(id) + ",";
        sql += QString::number(art->getID()) + ",";
        sql += "'" + art->getName() + "',";
        sql += QString::number(art->getQuantity()) + ",";
        sql += QString::number(art->getSinglePrice()) + ",";
        sql += QString::number(i) + ");";
        if (query.exec(sql)) {
            art->lock();
            qDebug() << "saved article to db";
        } else {
            qDebug() << "saving article failed" << query.lastError().text() << sql;
        }
    }
}

void Sale::clear() {
    if (!this->locked) {
        // Delete all data from the model
        beginResetModel();
        articles.clear();
        endResetModel();

        // clear everything from the database
        // if something already existed
        QSqlQuery query;
        if (id != -1) {
            query.exec("DELETE FROM sale WHERE id = " + QString::number(id) + ";");
            query.exec("DELETE FROM sale_article WHERE sale_id = " + QString::number(id) + ";");
            id = -1;
        }

        emit totalChanged(getTotal());
    }
}
void Sale::addArticle(Article* art)  {
    if (!locked) {
        beginInsertRows(QModelIndex(),articles.length(),articles.length());
        //If it was the first article that is added, safe the datetime
        if (articles.count() == 0) {
            this->timestampStart = QDateTime::currentDateTime();
            emit startTimeIsUpdated(this->timestampStart);
            dbInit();
        }

        this->articles.append(art);//Add the article to the internal list
        QObject::connect(art,SIGNAL(dueToDelete()),this,SLOT(deleteArticle()));//Connect the delete signal so the empty row in the model can be updated
        QObject::connect(art,SIGNAL(totalPriceChanged()),this,SLOT(updateTotal()));
        QObject::connect(art,SIGNAL(quantityChanged(float)),this,SLOT(refresh()));

        //if the article that was added is an offer emit a message
        if (art->isOffer()) {
            emit newMessage("im Angebot bis " + art->getOfferEndDate().toString("dd.MM.yy") + "\nRegulär " + QString::number(art->getNormalPrice(),'f',2) + " €");
        } else if(!art->isDeposit()) {
            emit newMessage("Regulärer Artikel");
        }

        refreshColorList();
        emit totalChanged(this->getTotal());
        emit articleCountChanged(this->getArticleCount());
        emit takebackCountChanged(this->getTakebackCount());
        endInsertRows();
    }
}
void Sale::updateTotal() {
    emit totalChanged(this->getTotal());
}
void Sale::refresh() {
    emit dataChanged(index(0,0),index(articles.length(),6));
    emit articleCountChanged(this->getArticleCount());
    emit takebackCountChanged(this->getTakebackCount());
}
bool Sale::isLocked() {
    return locked;
}


void Sale::deleteArticle() {
    //Every time an article object is deleted this function is triggered
    //The model therefore contains an empty entry. Search for that entry
    //and delete it.
    for(int i=0;i < articles.length(); i++) {
        if (articles.at(i)->isAboutToDelete()) {
            beginRemoveRows(QModelIndex(),i,i);
            articles.removeAt(i);
            endRemoveRows();
            break;
        }
    }
    //As this affects the total emit the signal
    refreshColorList();
    emit totalChanged(this->getTotal());
    emit articleCountChanged(this->getArticleCount());
    emit takebackCountChanged(this->getTakebackCount());
}
QModelIndex Sale::index(int row, int column, const QModelIndex & parent) const {
    if (!parent.isValid() && row < articles.length())
        return createIndex(row,column,articles.at(row));
    else
        return QModelIndex();
}
QModelIndex Sale::parent(const QModelIndex & index) const {
    return QModelIndex();
}
int Sale::rowCount(const QModelIndex & parent) const {
    return articles.length();
}
int Sale::columnCount(const QModelIndex & parent) const {
    return 6;
}
QVariant Sale::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == 1)
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        else
            return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return (static_cast<Article*>(index.internalPointer()))->getPLU();
        else if (index.column() == 1)
            return "   "  + (static_cast<Article*>(index.internalPointer()))->getName();
        else if (index.column() == 2)
            return (static_cast<Article*>(index.internalPointer()))->getQuantity();
        else if (index.column() == 3)
            return QString::number((static_cast<Article*>(index.internalPointer()))->getSinglePrice(),'f',2) + " €";
        else if (index.column() == 4)
            return QString::number((static_cast<Article*>(index.internalPointer()))->getTotalPrice(),'f',2) + " €";
        else if (index.column() == 5)
            return static_cast<Article*>(index.internalPointer())->getUnit();
        else
            return "";

    } else if (role == Qt::BackgroundColorRole) {
        if (color.length() > index.row()) {
            if (color.at(index.row())) {
                QColor c(255,255,255);
                return QVariant( c );
            } else {
                QColor c(230,230,230);
                return QVariant( c );
            }
        }
    } else if (role == Qt::FontRole) {
        QFont font;
        Article* article;
        article = static_cast<Article*>(index.internalPointer());

        if (!article->isDeposit() && !article->isTakeBack()) {
            font.setBold(true);
        }
        if (!article->isTakeBack()) {
            font.setItalic(true);
        }
        return QVariant(font);
    }
    return QVariant();
}
bool Sale::hasChildren(const QModelIndex & parent) const {
    if (parent.isValid())
        return false;
    else
        return true;
}
QVariant Sale::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return "PLU";
        } else if (section == 1) {
            return "Bezeichnung";
        } else if (section == 2) {
            return "Menge";
        } else if (section == 3) {
            return "Einzelpreis";
        } else if (section == 4) {
            return "Gesamt";
        } else {
            return "";
        }
    }
    return QVariant();
}
void Sale::refreshColorList() {
    bool oldColor = true;

    color.clear();
    for (int i=0; i<articles.length(); i++) {
        if (!articles.at(i)->isDeposit())
            oldColor = !oldColor;
        color.append(oldColor);
    }
}

float Sale::getGivenMoney() {
    return this->givenMoney;
}

float Sale::getArticleCount()
{
    float sum=0;
    Article* a;
    for (int i=0; i<(this->articles.length()); i++) {
        a = this->articles.at(i);
        if (!a->isDeposit() && !a->isTakeBack()) {
            sum = sum + a->getQuantity();
        }
    }
    return sum;
}

float Sale::getTakebackCount()
{
    float sum=0;
    Article* a;
    for (int i=0; i<(this->articles.length()); i++) {
        a = this->articles.at(i);
        if (!a->isDeposit() && a->isTakeBack()) {
            sum = sum + a->getQuantity();
        }
    }
    return sum;
}
