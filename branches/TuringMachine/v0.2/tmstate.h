#ifndef TMSTATE_H
#define TMSTATE_H

#include <QString>
#include <QMap>

class TMState
{
private:
    TMState(const QString name, const bool halt=false);
public:


    bool halt;
    QString name;

    static QString first_state;
    static QMap<QString,TMState> map;
    static void clear();
    static void add(const QString name);
};

#endif // TMSTATE_H
