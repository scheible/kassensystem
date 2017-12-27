#ifndef CUSTOMERSEARCHTABLEMODEL_H
#define CUSTOMERSEARCHTABLEMODEL_H

#include <QAbstractTableModel>
#include "customer.h"

class CustomerSearchTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<Customer*> customers;

    void deleteAllCustomers();

public:
    CustomerSearchTableModel();
    ~CustomerSearchTableModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Customer* getCustomer(int index);

public slots:
    void search(QString);

};

#endif // CUSTOMERSEARCHTABLEMODEL_H
