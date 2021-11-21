#ifndef Token_H
#define Token_H
#include <string>
#include <vector>
#include <ctype.h>


class Token
{
private:
    std::string raw;
    int lineNum;
    std::string type;

public:
    Token(){};

    Token(std::string tokenType, std::string input, int lineNumber)
    {
        this->type = tokenType;
        this->raw = input;
        this->lineNum = lineNumber;
    }
    std::string toString()
    {
        std::string full = "(" + type + ",\"" + raw + "\"," + std::to_string(lineNum) + ")";
        return (full);
    }
    std::string GetType() 
    {
        return type;
    }
    std::string GetRaw()
    {
        return raw;
    }
};

#endif