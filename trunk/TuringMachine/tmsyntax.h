#ifndef TMSYNTAX_H
#define TMSYNTAX_H

#include <QSyntaxHighlighter>
class QTextEdit;

class TMSyntax : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit TMSyntax(QTextEdit *parent = 0);
    
    void highlightBlock(const QString &text);
    QRegExp get_valid_exp();
signals:
    
public slots:

private:
    QFont font;
    QTextCharFormat format_white;
    QTextCharFormat format_comment;
    QTextCharFormat format_error;
    QTextCharFormat format_state;
    QTextCharFormat format_character;
    QTextCharFormat format_command;

    QRegExp ex_state;
    QRegExp ex_alphabet;
    QRegExp ex_command;
    QRegExp ex_line;
    QRegExp ex_comment;
    QRegExp ex_white;

};

#endif // TMSYNTAX_H
