#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T23:01:30
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kasse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    article.cpp \
    articlefactory.cpp \
    sale.cpp \
    categorybuttons.cpp \
    quickbutton.cpp \
    freearticle.cpp \
    changequantity.cpp \
    paydialog.cpp \
    keyeventdetector.cpp \
    customer.cpp \
    searchcustomer.cpp \
    customersearchtablemodel.cpp \
    newcustomer.cpp \
    bonprinter.cpp \
    iniparser.cpp \
    customerdialog.cpp \
    reporting.cpp

HEADERS  += mainwindow.h \
    article.h \
    articlefactory.h \
    sale.h \
    categorybuttons.h \
    quickbutton.h \
    freearticle.h \
    changequantity.h \
    paydialog.h \
    keyeventdetector.h \
    customer.h \
    searchcustomer.h \
    customersearchtablemodel.h \
    newcustomer.h \
    bonprinter.h \
    iniparser.h \
    customerdialog.h \
    reporting.h

FORMS    += mainwindow.ui \
    freearticle.ui \
    changequantity.ui \
    paydialog.ui \
    searchcustomer.ui \
    newcustomer.ui \
    customerdialog.ui \
    reporting.ui

include(3rdparty/xlsx/qtxlsx.pri)
