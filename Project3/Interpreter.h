#ifndef Interpreter_H
#define Interpreter_H
#include "Database.h"
#include <iostream>
#include "Token.h"
#include "Rule.h"
#include "Parameter.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Domain.h"
#include <vector>
#include <string>
#include <set>

class Interpreter
{
private:
    Database* myBase;
    DatalogProgram *prog;
public:
    Interpreter(DatalogProgram *input)
    {
        prog = input;
    };

    void makeDatabase()
    {
        myBase = new Database(prog);
    };

    void printTest()
    {
        myBase->printRelations();
        return;
    };
    // Do I pass through a pointer? constant?If so, how do I access the names, etc?
    Relation evaluatePredicate(const Predicate& preddy)
    {
        std::string stringer = "'string'";
         // get name to match relation
        // Predicate* pp = &preddy;
        std::vector<std::string> varStringVector;
        std::vector<int> varIntVector;

        std::map<std::string, int> stringIntMap;
        // std::cout << "Working on predicate ->  " << preddy.ToString() << std::endl;
        Relation modRelation = myBase->getRelation(preddy.getName());

        std::vector<std::string> queryParam = preddy.getParam();

        // Hmmm I worry about the coopy of the relation that I am modifying. I am trying to make a new copy with getRelation() not sure if all the objects inside are completely new as well or not. 


        for (unsigned int i = 0; i < queryParam.size(); i++)
        {
            if (queryParam[i][0] == stringer[0])
            {
                // We got a constant!
                
                modRelation = modRelation.columnSelect(i, queryParam[i]);
            }
            else 
            {
                // We got a variable!
                std::string tempVariable = queryParam[i];
                
                //Have we seen this variable before?
                if (stringIntMap.find(tempVariable) != stringIntMap.end())
                {
                    // We have seen this variable before...
                    modRelation = modRelation.doubleSelect(stringIntMap[tempVariable], i);
                      
                }
                else 
                {
                    // We have not seen this variable before
                    // std::cout << "We have not seen this variable before" << tempVariable << std::endl;
                    stringIntMap[tempVariable] = i;
                    varStringVector.push_back(tempVariable);
                    varIntVector.push_back(i);
                }
            }
        }

        // Here we will run a project.
        modRelation = modRelation.project(varIntVector);



        // Here we will run rename. 
        modRelation = modRelation.rename(varStringVector);

        return modRelation;
    }

    void runQueries()
    {
        std::vector<Predicate*> myQueries = prog->getQuerypred();
        for (unsigned int i = 0; i < myQueries.size(); i++)
        {
           

            // hmmm SHOULD I BE DEREFERRENECING???
            Relation newEval = evaluatePredicate(*myQueries[i]);
            
            // Print out newEval
            // TODO 
            std::cout << myQueries[i]->ToString() << "? ";
            // std::cout << "Macaroni... With the chicken strips..." << std::endl;
            newEval.printRes();
            // std::cout << "OHHHH" << std::endl;
        }

    }
};

#endif