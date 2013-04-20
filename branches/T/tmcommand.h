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
  int line_defined;

  void set(const TMCOM_TYPE type, const QString write_char, const QString goto_state, const int line_defined);

  //static QMap< QString,QMap<QString,TMCommand> > map;

  //static void clear();
  //static void queue_add(const QStringList s_list, const int line_defined);
  //static int add();

  TMCommand operator=(const TMCommand command);
private:
  //static QList<QStringList> queue_list;
  //static QList<int> queue_line;
};

#endif // TMCOMMAND_H
