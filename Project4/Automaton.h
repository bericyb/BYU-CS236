#ifndef Automaton_H
#define Automaton_H
#include <string>
#include <vector>
#include "Token.h"
#include <ctype.h>

class Automaton
{
private:
protected:
    int inputRead = 0;
    int newLines = 0;
    std::string newLine = "\n";
    std::string whiteSpace = " ";
    // const char ender = std::char_traits<wchar_t>::eof();
public:
    std::string type = "";
    Automaton(){};

    Automaton(std::string tokenType)
    {
        this->type = tokenType;
    };

    virtual int Start(const std::string &input) = 0;

    virtual Token *CreateToken(const std::string input, int lineNumber)
    {
        return (new Token(type, input, lineNumber));
    }

    virtual int NewLinesRead() const
    {
        return newLines;
    }
};

class MatcherAutomaton : public Automaton
{
private:
    std::string toMatch;

public:
    MatcherAutomaton(std::string tokenType, std::string toMatch)
    {
        this->type = tokenType;
        this->toMatch = toMatch;
    };

    int Start(const std::string &input)
    {

        bool isMatch = true;
        inputRead = 0;
        for (unsigned int i = 0; i < toMatch.size() && isMatch; i++)
        {
            if (input[i] != toMatch[i])
            {
                isMatch = false;
            }
        }
        if (isMatch)
        {
            inputRead = toMatch.size();
            return inputRead;
        }
        else
        {
            return 0;
        }
    }
    int NewLinesRead()
    {
        return newLines;
    }
};

class StringAutomaton : public Automaton
{
private:
    std::string doubleQ = "\"";
    std::string singleQ = "\'";
    std::string newLine = "\n";

public:
    StringAutomaton(std::string tokenType)
    {
        this->type = tokenType;
    };

    int Start(const std::string &input)
    {
        // Assuming is it a string?? HMMMM?
        // std::cout << "In string start" << std::endl;
        // std::cout << doubleQ << std::endl;
        ////std::cout << singleQ << std::endl;
        bool isString = true;
        inputRead = 0;
        newLines = 0;
        if (input[0] == doubleQ[0])
        {
            ////std::cout << "Found a double quote!" << std::endl;
            ////std::cout << "hello" << std::endl;
            inputRead++;
            unsigned int i = 1;
            while (isString)
            {
                if (input.size() < i)
                {
                    // Unfinished string
                   //std::cout << -inputRead << std::endl;
                    return -inputRead;
                }
                if (input[i] == doubleQ[0] && i == 1)
                {
                    // std::cout << "This is the empty string which is apparently invalid!" << std::endl;
                    inputRead = 0;

                    isString = false;
                    // std::cout << "HUH?" << std::endl;
                    return 0;
                    break;
                    // std::cout << "WHAT???" << std::endl;
                }
                // std::cout << "In string while" << std::endl;
                // We may have to deal with in line quotes???? HMMMM
                if (input[i] == doubleQ[0])
                {
                    // std::cout << "Found a second double quote" << std::endl;
                    isString = false;
                    inputRead++;
                    break;
                    return inputRead;
                }
                else if (input[i] == newLine[0])
                {
                    newLines++;
                    i++;
                    inputRead++;
                }
                else if (input.size() <= i)
                {
                    isString = false;
                    inputRead = 0;
                    break;
                    return 0;
                }
                else
                {
                    i++;
                    inputRead++;
                }
            }
            return inputRead;
        }
        else if (input[0] == singleQ[0])
        {
            // std::cout << "Found a single quote!" << std::endl;
            // std::cout << "hello" << std::endl;
            inputRead++;
            unsigned int i = 1;
            unsigned int internal = 1;
            while (isString)
            {
                if (input.size() < i)
                {
                    // Unfinished string!
                   //std::cout << -inputRead << std::endl;
                    return (-inputRead);
                }
                if (input[inputRead] == singleQ[0])
                {
                    // could be last.
                    internal++;
                }
                if (internal % 2 == 0 && (input.size() < i || input[inputRead + 1] != singleQ[0]))
                {
                    // string could be closed
                    isString = false;
                    inputRead++;
                    break;
                    return inputRead;
                }
                if (input[inputRead] == newLine[0])
                {
                    newLines++;
                    // inputRead++;
                }
                inputRead++;
                i++;
            }
            return inputRead;
        }
        else
        {
            return 0;
        }
        return 0;
    }
    int NewLinesRead()
    {
        return newLines;
    }
};

class IdentifierAutomaton : public Automaton
{
private:
    std::vector<std::string> keywords = {"Schemes", "Facts", "Rules", "Queries"};

public:
    IdentifierAutomaton(std::string tokenType)
    {
        this->type = tokenType;
    };

    int Start(const std::string &input)
    {
        newLines = 0;
        int inputRead = 0;
        bool isID = true;
        if (std::isalpha(input[0]))
        {
            inputRead++;
            while (isID)
            {
                //std::cout << input[inputRead] << "Current Reading" << std::endl;
                if (isalnum(input[inputRead]) == false)
                {
                    //std::cout << "Bad char!" << std::endl;
                    break;
                }
                if (input.size() < 1)
                {
                    return 0;
                }
                else if (input[inputRead] == newLine[0])
                {
                    return inputRead;
                }
                else if (input[inputRead] != whiteSpace[0])
                {
                    inputRead++;
                }
                else if (input[inputRead] == whiteSpace[0])
                {
                    // std::cout << "What the heck" << isalnum(input[inputRead]) << std::endl;
                    // inputRead++;
                    break;
                }
            }
            for (unsigned int i = 0; i < keywords.size(); i++)
            {
                if (keywords[i] == input.substr(0, inputRead))
                {
                    return 0;
                }
            }
            return inputRead;
        }
        return 0;
    }
};

class CommentAutomaton : public Automaton
{
private:
    std::string commentChars = "#|";
    std::string endMultiLineChar = "|#";
    std::string newLineChar = "\n";
    std::string tabber = "\t";
public:
    CommentAutomaton(std::string tokenType)
    {
        this->type = tokenType;
    };

    int Start(const std::string &input)
    {
        // Consider multi line comments and single line comment terminations. #| **** |# = multi-line # ***** = single line
        bool isComment = true;
        inputRead = 0;
        newLines = 0;
        //std::cout << input.substr(0,2) << std::endl;
        // First check for the mult line comment symbol.
        if (input.substr(0, 2) == commentChars)
        {
            inputRead = 2;
            //std::cout << "Found the commentChars" << std::endl;
            while (isComment)
            {
               //std::cout << input.substr(inputRead) << std::endl;
                if (input.substr(inputRead).size() < 1)
                {
                   //std::cout << "ENDOFFILE" << std::endl;
                    inputRead++;
                    return (-inputRead);
                }
                if (input.substr(inputRead, 2) == endMultiLineChar)
                {
                    //std::cout << "Found the end comment Char's bar" << std::endl;
                    inputRead++;
                    //std::cout << "Found the end comment Pound" << std::endl;
                    inputRead++;
                    break;
                }
                else if (input[inputRead] == newLineChar[0])
                {
                    newLines++;
                    inputRead++;
                }
                else if (input.size() < 1)
                {
                   //std::cout << "ENDOFFILE" << std::endl;
                    return (-inputRead);
                }
                else
                {
                    inputRead++;
                }
            }
            return inputRead;
        }
        else if (input[0] == commentChars[0])
        {
            inputRead++;
            // std::cout << "Found the single line comment Chars" << std::endl;
            while (isComment)
            {
                if (input[inputRead] == newLineChar[0] || input.size() < 1)
                {
                    // ending the comment!
                    return inputRead;
                }
                inputRead++;
            }
        }
        return 0;
    }
    virtual Token *CreateToken(const std::string input, int lineNumber)
    {
        std::string temp = input;
        for (unsigned int i = 0; i < temp.size(); i++)
        {
            if (temp[i] == newLineChar[0])
            {
                temp[i] = temp[i];
            }
            else if (temp[i] == tabber[0]) {
                temp[i] = temp[i];
            }
            else if (isspace(temp[i]))
            {
                temp[i] = whiteSpace[0];
            }
        }
        return (new Token(type, temp, lineNumber));
    }
};

class UndefinedAutomaton : public Automaton
{
private:
public:
    UndefinedAutomaton(std::string tokenType)
    {
        this->type = tokenType;
    };
    int Start(const std::string &input)
    {
        // bool isUndef = true;
        // unsigned int i = 0;
        // while (isUndef)
        // {
        //     if (input[i] == whiteSpace[0] || input.size() < i)
        //     {
        //         return inputRead;
        //     }
        //     if (input[i] == newLine[0])
        //     {
        //         newLines++;
        //     }
        //     i++;
        //     inputRead++;
        // }
        // return inputRead;
        return input.size();
    }
};

#endif