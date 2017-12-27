#ifndef BONPRINTER_H
#define BONPRINTER_H

#include <QObject>
#include "sale.h"

class BonPrinter : public QObject
{
    Q_OBJECT
public:
    explicit BonPrinter(Sale *s, QObject *parent = 0);
    void print();


private:
    Sale *sale;
    QString headerTemplate;
    QString itemTemplate;
    QString footerTemplate;

    QString loadTextFromFile(const QString filename);
    void loadTemplatesFromFiles();
    QString fillPlaceholders(QString);

signals:

public slots:
};

#endif // BONPRINTER_H
