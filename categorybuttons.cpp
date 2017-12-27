#include "categorybuttons.h"
#include "quickbutton.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

CategoryButtons::CategoryButtons(QObject *parent) : QObject(parent)
{

}
void CategoryButtons::setupButtons(QGridLayout *layout) {
    int col, row;
    QFont *font = new QFont();
    font->setPointSize(16);

    for (int i=0; i<(height*width); i++) {
        col = i%width;
        row = i/width;
        QuickButton *q = new QuickButton();
        layout->addWidget(q,row,col);
        q->setMaximumHeight(500);
        q->setFont(*font);
        this->buttons.append(q);
        QObject::connect(q,SIGNAL(category_clicked(int)),this,SLOT(openCategory(int)));
        QObject::connect(q,SIGNAL(plu_clicked(int)),this,SLOT(article_click(int)));
    }
    delete font;
}
int CategoryButtons::getElementCount() {
    return this->categories.length() + this->articles.length();
}
void CategoryButtons::setScrollBar(QScrollBar* scrollBar) {
    this->scrollBar = scrollBar;
    this->scrollBar->setSingleStep(1);
    this->scrollBar->setPageStep(height);
    QObject::connect(scrollBar,SIGNAL(valueChanged(int)),this,SLOT(update(int)));
}
void CategoryButtons::disableAllButtons() {
    for (int i=0; i<buttons.length(); i++) {
        this->buttons.at(i)->setDisabled(true);
        this->buttons.at(i)->setText("");
    }
}
int CategoryButtons::extendingRows() {
    //How many rows are needed additionally, that have to be added to the scrollbar?
    int nAll = getElementCount();
    int extendingElements = nAll - (width*height);
    int rest = extendingElements % width;
    int extendingRows = extendingElements / width;
    if (rest > 0) {
        extendingRows += 1;
    }
    if (extendingRows > 0) {
        return extendingRows;
    } else {
        return 0;
    }
}
int CategoryButtons::getScrollbarLength() {
    return extendingRows();
}
void CategoryButtons::update(int rowShift) {
    int buttonShift = rowShift * width;
    int nCat = this->categories.length();
    int nArt = this->articles.length();
    int nAll = nCat + nArt;
    int nBut = this->buttons.length();

    QuickButton *button;
    IdLabel *data;

    disableAllButtons();

    //Map the buttons according to the categories
    for (int i=0; (i+buttonShift)<nCat && i<nBut; i++) {
        button = this->buttons.at(i);
        data = this->categories.at(i+buttonShift);

        button->setText(data->label);
        button->setEnabled(true);
        button->setCategory(data->id);
    }

    int firstArticleButton = nCat-buttonShift;
    if (firstArticleButton < 0)
        firstArticleButton = 0;

    //Map the buttons to the articles
    for (int i=firstArticleButton; i>=0 && (i+buttonShift)<nAll && i<nBut; i++) {
        button = this->buttons.at(i);
        data = this->articles.at(i-(nCat-buttonShift));

        button->setText(data->label);
        button->setEnabled(true);
        button->setPlu(data->id);
    }

    scrollBar->setMaximum(getScrollbarLength());
}
void CategoryButtons::search(QString searchTerm) {
    QSqlQuery query;

    //breadCrumbs.clear();
    if (query.exec("SELECT plu, name FROM article WHERE name LIKE '%" + searchTerm + "%' OR plu LIKE '%"+ searchTerm +"%';")) {

        categories.clear();
        articles.clear();
        IdLabel *a;
        while (query.next()) {
            qDebug() << "found an article";
            a = new IdLabel;
            a->id = query.value(0).toInt();
            a->label = query.value(1).toString();
            this->articles.append(a);
        }
    } else {
        qDebug() << "search failed";
    }
    update();
}

void CategoryButtons::openCategory(int categoryId) {
    QSqlQuery query;

    if (query.exec("SELECT * FROM category WHERE parentId = " + QString::number(categoryId) + ";")) {

        categories.clear();
        IdLabel *c;
        while (query.next()) {
            c = new IdLabel;
            c->id = query.value(0).toInt();
            c->label = query.value(1).toString();
            this->categories.append(c);
        }

        if (categoryShown >= 0) {
            breadCrumbs.append(categoryShown);
        }
        categoryShown = categoryId;


    } else {
        qDebug() << "Categories could not be loaded from database!";
    }

    if (query.exec("SELECT plu, name FROM article WHERE categoryId =" + QString::number(categoryId) + ";")) {
        articles.clear();
        IdLabel *a;
        while (query.next()) {
            a = new IdLabel;
            a->id = query.value(0).toInt();
            a->label = query.value(1).toString();
            this->articles.append(a);
        }
    }
    update();
}
void CategoryButtons::article_click(int plu) {
    emit addArticlePlu(plu,1);
}
void CategoryButtons::goBack() {
    if (!breadCrumbs.isEmpty()) {
        openCategory(breadCrumbs.takeLast());
        breadCrumbs.removeLast();
    }
}
