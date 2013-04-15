#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include <QString>
#include <QMap>

enum TMCOM_TYPE{TMCOM_LEFT,TMCOM_RIGHT,TMCOM_WRITE,TMCOM_ERROR,TMCOM_NULL};

class TMCommand
{
public:
    TMCommand();

    TMCOM_TYPE type;
    QString write_char;
    QString goto_state;
    int line_defined;

    void set(TMCOM_TYPE type, QString write_char, QString goto_state, int line_defined);

    static QMap< QString,QMap<QString,TMCommand> > map;

    static void clear();
    static void queue_add(QStringList s_list, int line_defined);
    static int add();

    static int line_redefined;

private:
    static QList<QStringList> queue_list;
    static QList<int> queue_line;
};

#endif // TMCOMMAND_H
