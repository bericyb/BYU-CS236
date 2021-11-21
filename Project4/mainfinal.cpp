#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include "Parser.h"
#include "Lexer.h"
#include "Interpreter.h"



int main(int argc, char *argv[])
{
    // std::cout << "Hey what the is going on?" << std::endl;
    std::string filename = argv[1];
    std::ifstream ifs(filename);

    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    // std::cout << content << std::endl;

    Lexer *lexer = new Lexer();
    // std::cout << "lexer created!" << std::endl;
    std::vector<std::string> results = lexer->Read(content);

    // for (unsigned int i = 0; i < results.size(); i++)
    // {
    //     std::cout << results[i] << std::endl;
    // }
    // std::cout << "Total Tokens = " << results.size();

    std::vector<Token*> tokenVec = lexer->TokenVec();
    std::vector<Token*> noComments;

    for (unsigned int i = 0; i < tokenVec.size(); i++) 
    {
        if (tokenVec[i]->GetType() != "COMMENT")
        {
            noComments.push_back(tokenVec[i]);
        }
    }

    Parser *parser = new Parser(noComments);
    std::vector<std::string> parsed;
    parsed = parser->Run();
    if (parsed.size() == 0) 
    {
        return 0;
    }
    // std::cout << "Success!" << std::endl;
    // for (unsigned int j = 0; j < parsed.size(); j++)
    // {
    //     std::cout << parsed[j] << std::endl;
    // }

    // std::cout << "Getting the datalog data structures from the parser!" << std::endl;
    DatalogProgram *newProg = parser->getProgram();

    // std::cout << "Creating a new Interpreter!" << std::endl;
    Interpreter *interpreter = new Interpreter(newProg);

    // std::cout << "Making a new database" << std::endl;
    interpreter->makeDatabase();

    bool runner = true;
    int counter = 0;
    std::cout << "Rule Evaluation" << std::endl;
    while(runner)
    {
        runner = interpreter->runRules();
        counter++;
    }
    std::cout << std::endl;
    std::cout << "Schemes populated after " << counter << " passes through the Rules." << std::endl;
    std::cout << std::endl;
    std::cout << "Query Evaluation" << std::endl;
    interpreter->runQueries();

    return 0;
};