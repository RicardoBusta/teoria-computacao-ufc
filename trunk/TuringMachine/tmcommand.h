#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include <QString>
#include <QMap>

enum TMCOM_TYPE{TMCOM_LEFT,TMCOM_RIGHT,TMCOM_WRITE,TMCOM_ERROR};

class TMCommand
{
public:
    TMCommand();

    TMCOM_TYPE type;
    QString write_char;
    QString goto_state;

    void set(TMCOM_TYPE type, QString write_char, QString goto_state);

    static QMap< QString,QMap<QString,TMCommand> > map;
    static void clear();
    static bool add(QString from_state, QString from_character, QString goto_state, QString goto_command);
};

#endif // TMCOMMAND_H
