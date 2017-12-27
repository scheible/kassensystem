#ifndef CATEGORYBUTTONS_H
#define CATEGORYBUTTONS_H

#include <QObject>
#include <QSignalMapper>
#include <QPushButton>
#include <QScrollBar>
#include <QGridLayout>

#include "quickbutton.h"

struct IdLabel {
  int id;
  QString label;
} ;

class CategoryButtons : public QObject
{
    Q_OBJECT

private:
    const int width = 4;
    const int height = 5;
    QList<QuickButton*> buttons;
    QPushButton* backButton;
    QScrollBar* scrollBar;
    QList<IdLabel*> categories;
    QList<IdLabel*> articles;
    QList<int> breadCrumbs;
    int categoryShown = -1;

    void disableAllButtons();
    int getScrollbarLength();
    int extendingRows();


public:
    explicit CategoryButtons(QObject *parent = 0);
    void setupButtons(QGridLayout*);
    void setScrollBar(QScrollBar*);
    int getElementCount();

signals:
    void addArticlePlu(int plu, float quantity);

public slots:
    void update(int=0);
    void openCategory(int categoryId);
    void search(QString);
    void article_click(int plu);
    void goBack();

};

#endif // CATEGORYBUTTONS_H*/
