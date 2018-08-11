#include "articleselectordialog.h"

#include <QPushButton>
#include <QScrollArea>
#include <QLayout>


ArticleSelectorDialog::ArticleSelectorDialog(QWidget *parent) :
    QDialog(parent)
{
    list = new QListWidget(this);
    okButton = new QPushButton(this);
    cancelButton = new QPushButton(this);

    QFont *font = new QFont();
    font->setPointSize(16);

    list->setFont(*font);
    okButton->setFont(*font);
    cancelButton->setFont(*font);
    delete  font;

    okButton->setText("OK");
    cancelButton->setText("Abbrechen");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(list);
    vlayout->addLayout(layout);

    this->setLayout(vlayout);

    QObject::connect(okButton,SIGNAL(clicked()), this, SLOT(ok_clicked()));
    QObject::connect(cancelButton,SIGNAL(clicked()), this, SLOT(cancel_clicked()));


    list->setFocus();
    this->setMinimumWidth(600);
    this->setWindowTitle("Artikel auswählen");
    this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

ArticleSelectorDialog::~ArticleSelectorDialog()
{
    //delete ui;
}

void ArticleSelectorDialog::setArticles(QList<Article *> *artList)
{
    this->artList = artList;
    for (int i=0;i<artList->length();i++) {
        list->addItem(artList->at(i)->getName());
    }
    this->list->setCurrentRow(0);
}

Article *ArticleSelectorDialog::getSelectedArticle()
{
    return this->selectedArticle;
}

void ArticleSelectorDialog::deleteUnselectedArticles()
{
    for (int i=0; i<this->artList->length(); i++) {
        if (list->currentRow() != i) {
            delete artList->at(i);
        }
    }
}

void ArticleSelectorDialog::ok_clicked()
{
    this->selectedArticle = artList->at(list->currentRow());
    this->deleteUnselectedArticles();
    this->done(1);
}

void ArticleSelectorDialog::cancel_clicked()
{
    // before rejecting delete all articles
    for (int i=0; i<this->artList->length(); i++) {
        delete artList->at(i);
    }
    this->reject();
}
