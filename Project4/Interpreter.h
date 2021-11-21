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
    Database *myBase;
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
    Relation evaluatePredicate(const Predicate &preddy)
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
        std::vector<Predicate *> myQueries = prog->getQuerypred();
        for (unsigned int i = 0; i < myQueries.size(); i++)
        {
            Relation newEval = evaluatePredicate(*myQueries[i]);
            std::cout << myQueries[i]->ToString() << "? ";
            newEval.printRes();
        }
    }

    bool runRules()
    {
        std::vector<Rule *> myRules = prog->getRules();

        bool added = false;
        // For each rule
        for (unsigned int i = 0; i < myRules.size(); i++)
        {
            std::vector<Relation> rulesRelas;
            std::vector<Predicate *> oneRule = myRules[i]->getPreds();

            std::string ruleString = myRules[i]->ToString();
            std::cout << ruleString << "." << std::endl;

            // For each body predicate
            for (unsigned int j = 1; j < oneRule.size(); j++)
            {
                Relation ruleEval = evaluatePredicate(*oneRule[j]);
                rulesRelas.push_back(ruleEval);
            }
            // Take the first rule relation.
            if (rulesRelas.size() > 0)
            {
                Relation joinedRules = rulesRelas[0];
                // Next we join the rulesRelas
                for (unsigned int k = 1; k < rulesRelas.size(); k++)
                {
                    joinedRules = joinedRules.joiner(rulesRelas[k]);
                }

                // Project the columns of the head predicate
                Predicate *headPred = oneRule[0];
                // Take the parameters of headPred and find the indexes of those parameters in joinedRules.
                std::vector<int> finIndexes;
                std::vector<std::string> findIndexes =  headPred->getParam();
                Heady* joinedHeader = joinedRules.getHeader();
                std::vector<std::string> myHeader = joinedHeader->getHeader();
                for (unsigned int k = 0; k < findIndexes.size(); k++)
                {
                    for (unsigned int j = 0; j < myHeader.size(); j++)
                    {
                        if (findIndexes[k] == myHeader[j])
                            finIndexes.push_back(j);
                    }
                }

                // Call a project on joinedRules with the index vectors
                joinedRules = joinedRules.project(finIndexes);

                // Rename each of the attributes to match the header of the Relation the head predicate gives us.
                // I just copied the header of the OG relation to the new one. hmmmm

                // std::cout << "hmmm where should we print??" << std::endl;

                std::string ruleRelaName = headPred->getName();
                Relation *OGrelation = myBase->relationGetter(ruleRelaName);

                // std::cout << "Print test1" << std::endl;
                // std::vector<std::string> finalBoi = OGrelation->toString();
                // for (unsigned int i = 0; i < finalBoi.size(); i++)
                // {
                //     std::cout << finalBoi[i] << std::endl;
                // }
                // std::cout << "End test1 " << std::endl;


                Heady *headerCopy = OGrelation->getHeader();
                joinedRules.setHeader(headerCopy);

                // Union the Relation to the database with the matching relation
                if (OGrelation->unioner(joinedRules))
                {
                    added = true;
                }

                // std::cout << std::endl;
                // std::cout << "Print test2" << std::endl;
                // finalBoi = OGrelation->toString();
                // for (unsigned int i = 0; i < finalBoi.size(); i++)
                // {
                //     std::cout << finalBoi[i] << std::endl;
                // }

                // std::cout << "end test2" << std::endl;

               
            }
        }
        return added;
    }
};

#endif