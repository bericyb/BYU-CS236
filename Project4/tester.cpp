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

 
        
    std::cout << "End test print" << std::endl;

    // Starting natural join test.
    std::cout << "Starting natural join test" << std::endl;

    // Relation *test1 = new Relation("beric", {"A","B","C","D","E"});
    // test1->addRows({"a","b","c","d","e"});
    // test1->addRows({"a","a","a","a","a"});
    // // test1->addRows({"2","3","4","5"});
    // // test1->addRows({"2","3","4","5"});
    // // test1->addRows({"3","4","5","6"});

    // std::cout << "Making second relation" << std::endl;

    Relation *test2 = new Relation("Andre", {"B","E"});
    // test2->addRows({"a","a"});
    test2->addRows({"a","b"});
    // test2->addRows({"b","b"});
    // test2->addRows({"b","a"});


    Relation *test3 = new Relation("Jackson", {"C","E"});
    // test3->addRows({"a","a"});
    test3->addRows({"a","b"});
    // test3->addRows({"b","b"});
    // test3->addRows({"b","a"});

    Relation *test4 = new Relation("Sam", {"B","C","D"});
    test4->addRows({"a","a","b"});
    // test4->addRows({"a","b","a"});
    // test4->addRows({"a","b","b"});
    // test4->addRows({"b","a","a"});
    // test4->addRows({"b","a","b"});
    // test4->addRows({"b","b","a"});
    // test4->addRows({"b","b","b"});

    Relation *test5 = new Relation("Austin", {"A","D","E"});
    test5->addRows({"a","b","b"});
    // test5->addRows({"a","b","a"});
    // test5->addRows({"a","b","b"});
    // test5->addRows({"b","a","a"});
    // test5->addRows({"b","a","b"});
    // test5->addRows({"b","b","a"});
    // test5->addRows({"b","b","b"});



    std::cout << "Joining the relations" << std::endl;

    Relation res = test2->joiner(*test3);
    std::cout << "Print out new result!" << std::endl;
    std::vector<std::string> resString = res.toString();

    for (unsigned int i = 0; i < resString.size(); i++)
    {
        std::cout << resString[i] << std::endl;
    }

    std::cout << std::endl;
    res = res.joiner(*test4);
    std::cout << "Print out new result!" << std::endl;
    resString = res.toString();

    for (unsigned int i = 0; i < resString.size(); i++)
    {
        std::cout << resString[i] << std::endl;
    }

    res = res.joiner(*test5);
    std::cout << "Print out new result!" << std::endl;
    resString = res.toString();

    for (unsigned int i = 0; i < resString.size(); i++)
    {
        std::cout << resString[i] << std::endl;
    }

    // res = res.joiner(*test5);
    // std::cout << "Print out new result!" << std::endl;

    // resString = res.toString();

    // for (unsigned int i = 0; i < resString.size(); i++)
    // {
    //     std::cout << resString[i] << std::endl;
    // }

    

    std::cout << "End of natural join test" << std::endl;
}