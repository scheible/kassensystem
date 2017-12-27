#include "mainwindow.h"
#include <QApplication>
#include "keyeventdetector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QObject *keyEvDec = new keyEventDetector(&w);
    QCoreApplication::instance()->installEventFilter(keyEvDec);

    return a.exec();
}
