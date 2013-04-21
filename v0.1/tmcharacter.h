#ifndef TMCHARACTER_H
#define TMCHARACTER_H

#include <QString>
#include <QMap>

class TMCharacter
{
public:
    TMCharacter(QString name);

    QString name;

    static QMap<QString,TMCharacter> map;
    static void clear();
    static void add(const QString name);
};

#endif // TMCHARACTER_H
