#ifndef QUICKBUTTON_H
#define QUICKBUTTON_H
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QPainter>
#include <QColorDialog>

class QuickButton : public QPushButton
{
    Q_OBJECT

private:
    bool categoryFlag;
    int plu;
    int categoryId;

    QPixmap* getIcon(QColor);
    QAction* addColorAction(QColor, QString, QMenu*);

public:
    QuickButton();

    bool isCategory();
    bool isArticle();
    int getCategoryId();
    int getPlu();
    void setCategory(int categoryId);
    void setPlu(int plu);
    void setDisabled(bool);
    void setColor(QString color);
    void setColorWithSignal(QString color);

private slots:
    void this_clicked();
    void contextMenu(QPoint);

    void colorClickedClear();
    void colorClickedRed();
    void colorClickedGreen();
    void colorClickedBlue();
    void colorClickedYellow();
    void colorClickedCyan();
    void colorClickedMagenta();
    void otherColorClicked();

signals:
    void category_clicked(int);
    void plu_clicked(int);
    void color_changed(QString color, int id, bool categoryFlag);
};

#endif // QUICKBUTTON_H
