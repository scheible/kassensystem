#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QObject>

class Customer : public QObject
{
    Q_OBJECT

private:
    int id;
    QString company;
    QString firstName;
    QString lastName;
    QString title;
    QString street;
    QString number;
    QString zip;
    QString city;
    int category;
    QString phone;
    QString email;

public:
    explicit Customer(int id, QObject *parent = 0);
    Customer(int id, QString firstName, QString lastName, QObject *parent = 0);
    Customer(int id, QString firstName, QString lastName, QString title, QString street, QString number, QString zip, QString city, QString phone, QString email, QObject *parent = 0);

    int getId();
    QString getFirstName();
    QString getLastName();

    Customer* clone();

signals:

public slots:
    //void setCompany(QString);
    void setFirstName(QString);
    void setLastName(QString);
    //void setTitle(QString);
    //void setStreet(QString);
    //void setNumber(QString);
};

#endif // CUSTOMER_H
