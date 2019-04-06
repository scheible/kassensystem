#include "quickbutton.h"

QuickButton::QuickButton()
{
    this->categoryFlag = true;
    this->plu = 0;
    this->categoryId = 0;
    QObject::connect(this,SIGNAL(clicked()),this,SLOT(this_clicked()));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
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

    this->setStyleSheet("background-color: grey; font-weight: bold;");
}

void QuickButton::setPlu(int plu) {
    this->plu = plu;
    this->categoryFlag = false;
}
void QuickButton::setDisabled(bool b) {
    QPushButton::setDisabled(b);
    this->setStyleSheet("");
}
void QuickButton::setColor(QString color) {
    if (this->categoryFlag) {
        if (color == "")
            color = "gray";
        this->setStyleSheet("background-color:" + color + "; font-weight: bold;");
    } else {
        if (color == "")
            color = "none";
        this->setStyleSheet("background-color:" + color + ";");
    }
}

void QuickButton::setColorWithSignal(QString color)
{
    this->setColor(color);

    if (this->categoryFlag) {
        emit color_changed(color, getCategoryId(), true);
    } else {
        emit color_changed(color, getPlu(), false);
    }
}

void QuickButton::contextMenu(QPoint point)
{
    QMenu contextMenu("change color", this);
    contextMenu.setStyleSheet("background-color: none;");

    QAction actionClear("keine Farbe", this);
    contextMenu.addAction(&actionClear);

    QAction *actionRed = addColorAction(Qt::red, "Rot", &contextMenu);
    QAction *actionGreen = addColorAction(Qt::green,"Grün",&contextMenu);
    QAction *actionBlue = addColorAction(Qt::blue,"Blau",&contextMenu);
    QAction *actionYellow = addColorAction(Qt::yellow,"Gelb",&contextMenu);
    QAction *actionCyan = addColorAction(Qt::cyan,"Cyan",&contextMenu);
    QAction *actionMagenta = addColorAction(Qt::magenta,"Rosa",&contextMenu);

    QAction actionOtherColor("mehr Farbe", this);
    contextMenu.addAction(&actionOtherColor);


    this->connect(&actionClear,SIGNAL(triggered()),this,SLOT(colorClickedClear()));
    this->connect(&actionOtherColor,SIGNAL(triggered(bool)), this, SLOT(otherColorClicked()));

    this->connect(actionRed,SIGNAL(triggered()),this,SLOT(colorClickedRed()));
    this->connect(actionGreen,SIGNAL(triggered()),this,SLOT(colorClickedGreen()));
    this->connect(actionBlue,SIGNAL(triggered()),this,SLOT(colorClickedBlue()));
    this->connect(actionYellow,SIGNAL(triggered()),this,SLOT(colorClickedYellow()));
    this->connect(actionCyan,SIGNAL(triggered()),this,SLOT(colorClickedCyan()));
    this->connect(actionMagenta,SIGNAL(triggered()),this,SLOT(colorClickedMagenta()));

    contextMenu.exec(mapToGlobal(point));
    //delete actionRed;
    //delete actionGreen;
}

QAction* QuickButton::addColorAction(QColor color, QString text, QMenu *contextMenu)
{
    QAction* action = new QAction(text, this);
    contextMenu->addAction(action);
    QPixmap* pixmap = getIcon(color);
    QIcon *icon = new QIcon(*pixmap);
    action->setIcon(*icon);
    delete icon;
    delete pixmap;

    return action;
}
QPixmap *QuickButton::getIcon(QColor color)
{
    QImage qImage(10,10,QImage::Format_RGB16);
    QPainter qPainter(&qImage);
    QBrush brush(color);
    qPainter.fillRect(0,0,10,10,brush);
    bool bEnd = qPainter.end();
    QPixmap* pixmap = new QPixmap();
    pixmap->convertFromImage(qImage);
    return pixmap;
}
void QuickButton::colorClickedClear()
{
    //this->setStyleSheet("background-color: ;");
    this->setColorWithSignal("");
}

void QuickButton::colorClickedRed()
{
    //this->setStyleSheet("background-color: red;");
    this->setColorWithSignal("red");
}

void QuickButton::colorClickedGreen()
{
    //this->setStyleSheet("background-color: green;");
    this->setColorWithSignal("green");
}

void QuickButton::colorClickedBlue()
{
    //this->setStyleSheet("background-color: blue;");
    this->setColorWithSignal("blue");
}

void QuickButton::colorClickedYellow()
{
    //this->setStyleSheet("background-color: yellow;");
    this->setColorWithSignal("yellow");
}

void QuickButton::colorClickedCyan()
{
    //this->setStyleSheet("background-color: cyan;");
    this->setColorWithSignal("cyan");
}

void QuickButton::colorClickedMagenta()
{
    //this->setStyleSheet("background-color: magenta;");
    this->setColorWithSignal("magenta");
}

void QuickButton::otherColorClicked() {
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    this->setColorWithSignal(color.name());
}
