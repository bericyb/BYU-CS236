#ifndef Lexer_H
#define Lexer_H
#include <string>
#include <vector>
#include <ctype.h>
#include "Token.h"
#include "Automaton.h"

class Lexer
{
private:
    std::vector<Token *> tokens;
    std::vector<Automaton *> automata;
    std::string whiteSpace = " ";
    std::string newLineChar = "\n";
    std::string undef = "UNDEFINED";
    std::string tabber = "\t";
public:
    Lexer()
    {
        // This initialization might be a problem....
        tokens = std::vector<Token *>();
        automata = std::vector<Automaton *>();
        // Add in automatons
        automata.push_back(new MatcherAutomaton("COMMA", ","));
        automata.push_back(new MatcherAutomaton("PERIOD", "."));
        automata.push_back(new MatcherAutomaton("Q_MARK", "?"));
        automata.push_back(new MatcherAutomaton("LEFT_PAREN", "("));
        automata.push_back(new MatcherAutomaton("RIGHT_PAREN", ")"));
        automata.push_back(new MatcherAutomaton("COLON", ":"));
        automata.push_back(new MatcherAutomaton("COLON_DASH", ":-"));
        automata.push_back(new MatcherAutomaton("MULTIPLY", "*"));
        automata.push_back(new MatcherAutomaton("ADD", "+"));
        automata.push_back(new MatcherAutomaton("SCHEMES", "Schemes"));
        automata.push_back(new MatcherAutomaton("FACTS", "Facts"));
        automata.push_back(new MatcherAutomaton("RULES", "Rules"));
        automata.push_back(new MatcherAutomaton("QUERIES", "Queries"));
        automata.push_back(new StringAutomaton("STRING"));
        automata.push_back(new CommentAutomaton("COMMENT"));
        // automata.push_back(new IdentifierAutomaton("ID"));
        //std::cout << "machines" << std::endl;
        // automata.push_back(new ColonAutomaton());
        // automata.push_back(new ColonDashAutomaton());
    }

    std::vector<std::string> Read(std::string input)
    {
        int lineNum = 1;

        // std::cout << "In read!" << std::endl;
        // std::cout << input << std::endl;
        Automaton *maxFSA = automata[0];
        Automaton *minFSA = automata[0];
        int minRead = 0;
        while (input.length() > 0)
        {
            int maxRead = 0;
            Automaton *ID = new IdentifierAutomaton("ID");
            // std::cout << input << std::endl;
            // Take care of whitespaces...
            // Remove all of the whitespaces???
            while (input[0] == whiteSpace[0] || input[0] == tabber[0])
            {
                input = input.substr(1);
                // std::cout << input << std::endl;
            }
            // if (input[0] == whiteSpace[0]) {
            //     std::cout << "AYYYYY" << std::endl;
            //     input = input.substr(1);
            // }
            if (input[0] == newLineChar[0] && input.size() > 0)
            {
                // std::cout << "Found newLine Char" << std::endl;
                lineNum++;
                maxRead++;
                input = input.substr(1);
                // std::cout << input <<std::endl;
            }
            else if (maxRead == 0)
            {
                for (unsigned int i = 0; i < automata.size(); i++)
                {
                    int eval = automata[i]->Start(input);
                    // std::cout << eval << std::endl;
                    // std::cout << automata[i]->type << "automaton eval ^^^" << std::endl;
                    // std::cout << lineNum << "Line of token" << std::endl;
                    if (eval < minRead)
                    {
                       //std::cout << eval << std::endl;
                        minRead = eval;
                        minFSA = automata[i];
                    }
                    else if (eval > maxRead)
                    {
                        maxRead = eval;
                        maxFSA = automata[i];
                    }
                }
                int eval = ID->Start(input);
                if (eval > maxRead && eval > abs(minRead))
                {
                    maxRead = eval;
                    maxFSA = ID;
                    // std::cout << eval << "ID eval" << std::endl;
                    lineNum = lineNum + maxFSA->NewLinesRead();
                    // std::cout << lineNum << "current line num" << std::endl;
                    Token *newToken = maxFSA->CreateToken(input.substr(0, maxRead), lineNum - maxFSA->NewLinesRead());
                    tokens.push_back(newToken);
                    input = input.substr(maxRead);
                }
                else if (abs(minRead) > maxRead)
                {
                    Automaton *undef = new UndefinedAutomaton("UNDEFINED");
                    lineNum = lineNum + minFSA->NewLinesRead();
                    Token *newToken = undef->CreateToken(input.substr(0, abs(minRead)), lineNum - minFSA->NewLinesRead());
                    tokens.push_back(newToken);
                   //std::cout << "IS anybody out there?" << std::endl;
                    input = input.substr(abs(minRead) - 1);
                    minRead = 0;
                    // maxRead = abs(minRead);
                }
                else if (maxRead > 0 && input.size() > 0)
                {
                    // Taking out maxFSA->NewLinesRead();
                    // std::cout << maxFSA->NewLinesRead() << "current line num" << std::endl;
                    lineNum = lineNum + maxFSA->NewLinesRead();
                    // std::cout << lineNum << "current line num" << std::endl;
                    Token *newToken = maxFSA->CreateToken(input.substr(0, maxRead), lineNum - maxFSA->NewLinesRead());
                    tokens.push_back(newToken);
                    input = input.substr(maxRead);
                }
                else if (maxRead == 0)
                {
                    // std::cout << maxRead << "ID eval" << std::endl;
                    if (maxRead < 1 && input[0] != newLineChar[0] && input.size() > 1)
                    {
                        std::string temper = input;
                        unsigned int k = 1;
                        // std::cout << "Getting length till next valid..." << std::endl;
                        // while (eval < 1 && temper.size() > 2) {
                        //     std::cout << "temper: " << temper << std::endl;
                        //     temper = temper.substr(k);
                        //     for (unsigned int i = 0; i < automata.size(); i++)
                        //     {
                        //         int eval = automata[i]->Start(temper);
                        //         if (eval > 0) {
                        //             break;
                        //         }
                        //     }
                        //     eval = ID->Start(temper);
                        //     if (eval > 0) {
                        //         break;
                        //     }
                        //     k++;
                        // }
                        Automaton *undef = new UndefinedAutomaton("UNDEFINED");
                        Token *newToken = undef->CreateToken(input.substr(0, k), lineNum - undef->NewLinesRead());
                        tokens.push_back(newToken);
                       //std::cout << "Are we getting what we need? " << tokens.size() << std::endl;
                        input = input.substr(k);
                    }
                }
            }
            // break;
        }
        
        Token *end = new Token("EOF", "", lineNum);
        tokens.push_back(end);
        std::vector<std::string> finalTokens;
        for (unsigned int v = 0; v < tokens.size(); v++)
        {
            finalTokens.push_back(tokens[v]->toString());
        }
        // finalTokens.push_back(end->toString());
        return finalTokens;
    }
    // Other methods??
    std::vector<Token*> TokenVec() {
        return tokens;
    }
};

#endif