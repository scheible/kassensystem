#include "iniparser.h"
#include <QFileInfo>
#include <QTextStream>

IniParser::IniParser(const QString filePath)
{
    this->filePath = filePath;
    this->readFileIn(this->filePath);
}

IniParser::IniParser()
{
    // Overload the constructor with a standard filename for the ini file
    this->filePath = "settings.ini";
    this->readFileIn(this->filePath);
}

void IniParser::readFileIn(const QString filePath)
{
    //Open a text file
    QFile iniFile(filePath);
    if (iniFile.exists()) {
        QString line;
        QString fullFileContent = "";

        if (iniFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&iniFile);
            while (!stream.atEnd()){
                parseLine(stream.readLine());
            }
        }
        iniFile.close();
    }
}

void IniParser::parseLine(const QString line) {
    QString key;
    QString value;

    if (line.length() > 0 && line[0] != '#') {
        QStringList lineParts = line.split('=');
        if (lineParts.length() >= 2) {
            key = lineParts.at(0);
            value = lineParts.at(1);
            this->addKeyValuePair(key,value);
        }
    }
}

void IniParser::addKeyValuePair(const QString key, const QString value) {
    if (this->getKeyIndex(key) == -1) {
        KeyValuePair *newKp = new KeyValuePair({ key, value });
        this->KeyValuePairs.append(newKp);
    }
}

int IniParser::getKeyIndex(const QString key) {
    for (int i=0;i<this->KeyValuePairs.length();i++) {
        if (this->KeyValuePairs.at(i)->key == key) {
            return i;
        }
    }
    return -1;
}

QVariant IniParser::getValue(const QString key) {

    for (int i=0;i<this->KeyValuePairs.length();i++) {
        if (this->KeyValuePairs.at(i)->key == key) {
            return QVariant(this->KeyValuePairs.at(i)->value);
        }
    }
    return QVariant("");
}

bool IniParser::existsKey(const QString key) {
    for (int i=0;i<this->KeyValuePairs.length();i++) {
        if (this->KeyValuePairs.at(i)->key == key) {
            return true;
        }
    }
    return false;
}
