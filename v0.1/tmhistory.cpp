#include "tmhistory.h"

TMHistory::TMHistory(const QString state,const int head,const QString tape)
{
    this->state = state;
    this->head = head;
    this->tape = tape;
}

TMHistory TMHistory::operator =(const TMHistory h)
{
    this->state = h.state;
    this->head = h.head;
    this->tape = h.tape;
    return *this;
}
