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
signals:
    
public slots:

private:

};

#endif // TMSYNTAX_H
