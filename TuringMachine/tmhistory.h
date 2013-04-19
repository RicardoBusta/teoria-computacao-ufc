#ifndef TMHISTORY_H
#define TMHISTORY_H

#include <QString>

class TMHistory
{
public:
    int head;
    QString state;
    QString tape;

    TMHistory(const QString state, const int head, const QString tape);

    TMHistory operator=(const TMHistory h);
};

#endif // TMHISTORY_H
