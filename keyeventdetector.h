#ifndef KEYEVENTDETECTOR_H
#define KEYEVENTDETECTOR_H

#include <QObject>
#include "mainwindow.h"

class keyEventDetector : public QObject
{
    Q_OBJECT
private:
    MainWindow *mWindow;
public:
    explicit keyEventDetector(MainWindow *w,QObject *parent = 0);
    bool eventFilter(QObject *Object, QEvent *Event);

signals:

public slots:
};

#endif // KEYEVENTDETECTOR_H
