#include "customersearchtablemodel.h"
#include "customer.h"
#include <QSqlQuery>
#include <QColor>

CustomerSearchTableModel::CustomerSearchTableModel()
{

}
CustomerSearchTableModel::~CustomerSearchTableModel() {
    deleteAllCustomers();
}

void CustomerSearchTableModel::deleteAllCustomers() {
    while (customers.length() > 0) {
        delete customers.at(0);
        customers.removeFirst();
    }
}

void CustomerSearchTableModel::search(QString searchTerm) {
    QSqlQuery query;
    deleteAllCustomers();

    beginResetModel();
    for (int i = 0; i < customers.length(); i++) {
        delete customers.at(i);
    }
    customers.clear();
    if (query.exec("SELECT id, firstname, lastname FROM customer WHERE id LIKE '%" + searchTerm + "%' OR firstName LIKE '%" + searchTerm + "%' OR lastName LIKE '%" + searchTerm + "%';"))
    {
        while (query.next()) {
            Customer *c = new Customer( query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
            customers.append(c);
        }
    }
    endResetModel();
}

int CustomerSearchTableModel::rowCount(const QModelIndex & parent) const {
    if (customers.length() <= 0) {
        return 1;
    } else
        return customers.length();
}
int CustomerSearchTableModel::columnCount(const QModelIndex & parent) const {
    return 3;
}
QVariant CustomerSearchTableModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);

    } else if (role == Qt::DisplayRole) {
        if (index.row() < customers.length()) {
            if (index.column() == 0)
                return customers.at(index.row())->getId();
            else if (index.column() == 1)
                return customers.at(index.row())->getFirstName();
            else if (index.column() == 2)
                return customers.at(index.row())->getLastName();
        } else {
            if (index.column() == 0)
                return "Nichts gefunden";
        }
    } else if (role == Qt::BackgroundColorRole) {
        if (index.row() % 2 == 0) {
                QColor c(255,255,255);
                return QVariant( c );
         } else {
                QColor c(200,200,200);
                return QVariant( c );
        }
    }
    return QVariant();
}
Customer* CustomerSearchTableModel::getCustomer(int index) {
    if (index < customers.length()) {
        return customers.at(index)->clone();
    }
    return 0;
}
