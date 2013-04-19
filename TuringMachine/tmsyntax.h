#ifndef TMSYNTAX_H
#define TMSYNTAX_H

#include <QSyntaxHighlighter>
#include "exnamespace.h"

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
        inline token_s operator=(token_s t){this->start=t.start;this->count=t.count;this->type=t.type;return *this;}
    };

    token_s token_list[4];

    inline void break_line_token(QString valid_line){
        int pos=0;
        for(int i=0;i<4;i++){
            pos = io_ex::token.indexIn(valid_line,pos);
            if(i%2==0){
                token_list[i] = token_s(pos,io_ex::token.matchedLength(),io_ex::token_type_s(valid_line.mid(pos,io_ex::token.matchedLength())));
            }else{
                token_list[i] = token_s(pos,io_ex::token.matchedLength(),io_ex::token_type_c(valid_line.mid(pos,io_ex::token.matchedLength())));
            }
            pos = pos+io_ex::token.matchedLength();
        }
    }
};

#endif // TMSYNTAX_H
