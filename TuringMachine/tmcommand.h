#ifndef TMCOMMAND_H
#define TMCOMMAND_H

#include <QString>
#include <QMap>

enum TMCOM_TYPE{
    TMCOM_LEFT,
    TMCOM_RIGHT,
    TMCOM_WRITE,
    TMCOM_EXEC,
    TMCOM_ERROR
};

class TMCommand
{
public:
  TMCommand();

  TMCOM_TYPE type;
  QString write_char;
  QString goto_state;
  int line_defined;

  void set(const TMCOM_TYPE type, const QString write_char, const QString goto_state, const int line_defined);


  TMCommand operator=(const TMCommand command);
private:
};

#endif // TMCOMMAND_H
