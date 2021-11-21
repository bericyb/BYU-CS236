#ifndef Database_H
#define Database_H
#include <map>
#include <vector>
#include <string>
#include "Relation.h"
#include "Predicate.h"
#include "DatalogProgram.h"

class Database
{
private:
    std::map<std::string, Relation*> myRela;
public:
    Database(DatalogProgram* newProg)
    {
        // Creating new Relations from Schemes
        std::vector<Predicate*> mySchemes = newProg->getSchemepred();
        for (unsigned int i = 0; i < mySchemes.size(); i++)
        {
            // Get name for new Relation
            std::string tempName = mySchemes[i]->getName();
            // Get header for new Relation
            std::vector<std::string> tempParameters = mySchemes[i]->getParam();

            // Create new relation with name and header
            Relation *newRelation = new Relation(tempName, tempParameters);

            // Add relation to map based on name
            myRela[tempName] = newRelation;
        }

        // Adding in facts to relation 
        std::vector<Predicate*> myFacts = newProg->getFactpred();
        for (unsigned int i = 0; i < myFacts.size(); i++)
        {
            // Get name to match to Relation
            std::string tempName = myFacts[i]->getName();

            // Get Facts to add to the Relation 
            std::vector<std::string> tempParameters = myFacts[i]->getParam();

            // for (unsigned int i = 0; i < tempParameters.size(); i++)
            // {
            //     std::cout << tempParameters[i];
            // }
            // Find relation in Map and add rows to the relation
            Relation *relationMod = myRela[tempName];
            relationMod->addRows(tempParameters);
            // std::cout << "Successfully added rows??" << std::endl;
        }
        
        
    }

    Relation getRelation(std::string name)
    {
        // std::string nameyboi = name;
        // Heady* headeyboi = myRela[name]->getHeader();
        // std::set<Tupler> rowboi = myRela[name]->getRows();
        // Relation* newRela = new Relation(nameyboi, headeyboi, rowboi);

        return *myRela[name];
        // return *newRela;
    }

    void printRelations()
    {
        for (auto it=myRela.begin(); it!=myRela.end(); ++it)
        {
            // std::cout << "printing out realtion " << it->first << std::endl;
            std::vector<std::string> stringRelation = it->second->toString();
            for (unsigned int i = 0; i < stringRelation.size(); i++)
            {
                std::cout << stringRelation[i] << std::endl;
            }
        }
    }
};



#endif