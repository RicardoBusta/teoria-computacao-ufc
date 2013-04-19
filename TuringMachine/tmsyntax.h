#ifndef TMSYNTAX_H
#define TMSYNTAX_H

#include <QSyntaxHighlighter>
#include <exnamespace.h>

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
    struct token_s{
        int start;
        int count;
        io_ex::TOKEN type;

        inline token_s(int start=0, int count=0, io_ex::TOKEN type=io_ex::TOKEN_ERROR)
            :start(start),count(count),type(type){}
        inline token_s operator=(token_s t){this->start=t.start;this->count=t.count;this->type=t.type;}
    };

    token_s token_list[4];

    void break_line_token(QString valid_line){
        int pos=0;

        for(int i=0;i<4;i++){
            pos = io_ex::token.indexIn(valid_line,pos);
            qDebug() << "~~~~";
            qDebug() << pos << io_ex::token.matchedLength();
            token_list[i] = token_s(pos,io_ex::token.matchedLength(),io_ex::token_type(valid_line.mid(pos,io_ex::token.matchedLength())));
            qDebug() << token_list[i].start << token_list[i].count;
            pos = pos+io_ex::token.matchedLength();
        }
    }
};

#endif // TMSYNTAX_H
