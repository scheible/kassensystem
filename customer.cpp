#include "customer.h"

Customer::Customer(int id, QObject *parent) : QObject(parent)
{

}
Customer::Customer(int id, QString firstName, QString lastName, QString title, QString street, QString number, QString zip, QString city, QString phone, QString email, QObject *parent)  : QObject(parent) {
    this->id = id;
    this->firstName = firstName;
    this->lastName = lastName;
    this->title = title;
    this->number = number;
    this->zip = zip;
    this->city = city;
    this->phone = phone;
    this->email = email;
}

Customer::Customer(int id, QString firstName, QString lastName, QObject *parent) : QObject(parent) {
    this->id = id;
    this->firstName = firstName;
    this->lastName = lastName;
}

int Customer::getId() {
    return id;
}

QString Customer::getFirstName() {
    return firstName;
}

QString Customer::getLastName() {
    return lastName;
}

void Customer::setFirstName(QString name) {
    firstName = name;
}

void Customer::setLastName(QString name) {
    lastName = name;
}
Customer* Customer::clone() {
    return new Customer(id,firstName,lastName);
}
