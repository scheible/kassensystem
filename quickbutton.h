#ifndef QUICKBUTTON_H
#define QUICKBUTTON_H
#include <QPushButton>

class QuickButton : public QPushButton
{
    Q_OBJECT

private:
    bool categoryFlag;
    int plu;
    int categoryId;

public:
    QuickButton();

    bool isCategory();
    bool isArticle();
    int getCategoryId();
    int getPlu();
    void setCategory(int categoryId);
    void setPlu(int plu);
    void setDisabled(bool);

private slots:
    void this_clicked();

signals:
    void category_clicked(int);
    void plu_clicked(int);
};

#endif // QUICKBUTTON_H
