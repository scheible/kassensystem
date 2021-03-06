#ifndef INIPARSER_H
#define INIPARSER_H

#include <QString>
#include <QVariant>


struct KeyValuePair {
  QString key;
  QString value;
} ;

class IniParser
{

private:
    QString filePath;
    QList<KeyValuePair*> KeyValuePairs;

    void parseLine(const QString line);
    void addKeyValuePair(const QString key, const QString value);
    void readFileIn(const QString filePath);

public:
    IniParser(const QString filePath);
    IniParser();
    QVariant getValue(const QString key);
    int getKeyIndex(const QString key);
    bool existsKey(const QString key);
};

#endif // INIPARSER_H
