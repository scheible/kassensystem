#ifndef ARTICLESELECTORDIALOG_H
#define ARTICLESELECTORDIALOG_H




#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QDebug>
#include "article.h"

class ArticleSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArticleSelectorDialog(QWidget *parent = 0);
    ~ArticleSelectorDialog();
    QString getName(int count);
    void setArticles(QList<Article *> *artList);
    Article* getSelectedArticle();

private:
    QPushButton *cancelButton;
    QPushButton *okButton;
    QListWidget *list;
    QList<Article *> *artList;
    Article *selectedArticle;

    void deleteUnselectedArticles();

private slots:
    void ok_clicked();
    void cancel_clicked();
};

#endif // ARTICLESELECTORDIALOG_H
