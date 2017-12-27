#include "bonprinter.h"
#include <QDebug>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFile>

BonPrinter::BonPrinter(Sale *s, QObject *parent) : QObject(parent)
{
    this->loadTemplatesFromFiles();
    this->sale = s;
}
void BonPrinter::print() {
    int rows, cols;
    rows = this->sale->rowCount();
    cols = this->sale->columnCount();

    QString header = this->headerTemplate;
    QString receipt = "";
    header = this->fillPlaceholders(header);
    receipt.append(header);

    for (int i=0;i<rows;i++) {
        QString text = this->itemTemplate;

        QModelIndex index = sale->index(i,0);
        QVariant data = sale->data(index);
        text.replace("#PLU#",data.toString());

        index = sale->index(i,1);
        data = sale->data(index);
        text.replace("#NAME#",data.toString());

        index = sale->index(i,2);
        data = sale->data(index);
        text.replace("#QUANTITY#",data.toString());

        index = sale->index(i,3);
        data = sale->data(index);
        text.replace("#SINGLEPRICE#",data.toString());

        index = sale->index(i,4);
        data = sale->data(index);
        text.replace("#TOTALPRICE#",data.toString());

        receipt.append(text);
    }

    QString footer = this->footerTemplate;
    footer = this->fillPlaceholders(footer);
    receipt.append(footer);

    QTextDocument doc;

    QPrinter p(QPrinter::PrinterResolution);
    p.setOutputFormat(QPrinter::NativeFormat);
    QPrintDialog printDialog(&p);


    if (printDialog.exec() == QDialog::Accepted) {
        doc.setHtml(receipt);       
        p.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
        doc.print(&p);
    }

}

void BonPrinter::loadTemplatesFromFiles() {
    this->headerTemplate = this->loadTextFromFile("bon_template\\header_template.html");
    this->itemTemplate = this->loadTextFromFile("bon_template\\item_template.html");
    this->footerTemplate = this->loadTextFromFile("bon_template\\footer_template.html");

}
QString BonPrinter::loadTextFromFile(const QString filename) {
    QFile file(filename);

    if(!file.exists()){
        qDebug() << "Error loading bon template file. File not found! " << filename;
    }

    QString line;
    QString fullFileContent = "";

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&file);
            while (!stream.atEnd()){
                line = stream.readLine();
               fullFileContent = fullFileContent + "\n" + line;
            }
        }
    file.close();

    return fullFileContent;
}

QString BonPrinter::fillPlaceholders(QString tmpl) {
    tmpl.replace("#TOTAL#",QString::number(sale->getTotal(),'f',2) + " €");
    tmpl.replace("#DATE#",sale->getTimestampStart().toString("dd.MM.yyyy hh:mm"));
    tmpl.replace("#GIVEN#",QString::number(sale->getGivenMoney(),'f',2) + " €");
    tmpl.replace("#MWST#",QString::number(sale->getTotal()*(0.19/1.19),'f',2) + " €");
    tmpl.replace("#CHANGE#",QString::number(sale->getTotal() - sale->getGivenMoney(),'f',2) + " €");

    return tmpl;
}
