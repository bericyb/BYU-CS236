#ifndef Scheme_H
#define Scheme_H
#include "Parameter.h"

class Scheme
{
private:
    Predicate *preds;
    Token* name;
public:
    Scheme(Token* newName)
    {
        this->name = newName;
    }
};

#endif