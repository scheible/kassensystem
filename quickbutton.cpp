#include "quickbutton.h"

QuickButton::QuickButton()
{
    this->categoryFlag = true;
    this->plu = 0;
    this->categoryId = 0;
    QObject::connect(this,SIGNAL(clicked()),this,SLOT(this_clicked()));
}
void QuickButton::this_clicked() {
    if (this->isCategory()) {
        emit category_clicked(this->getCategoryId());
    } else {
        emit plu_clicked(this->getPlu());
    }
}

bool QuickButton::isCategory() {
    return this->categoryFlag;
}

bool QuickButton::isArticle() {
    return !this->categoryFlag;
}

int QuickButton::getCategoryId() {
    if (this->categoryFlag) {
        return this->categoryId;
    } else {
        return 0;
    }
}

int QuickButton::getPlu() {
    if (this->isArticle()) {
        return this->plu;
    } else {
        return 0;
    }
}

void QuickButton::setCategory(int categoryId) {
    this->categoryId = categoryId;
    this->categoryFlag = true;

    this->setStyleSheet("background-color: grey;");
}

void QuickButton::setPlu(int plu) {
    this->plu = plu;
    this->categoryFlag = false;
    this->setStyleSheet("background-color: blue;");
}
void QuickButton::setDisabled(bool b) {
    QPushButton::setDisabled(b);
    this->setStyleSheet("");
}