#ifndef Relation_h
#define Relation_h
#include "Heady.h"
#include "Tuple.h"
#include <set>
#include <string>

class Relation
{
private:
    std::string name;
    Heady *myHeader;

    std::set<Tupler> rows;

public:
    Relation(std::string nameIn, std::vector<std::string> headerIn)
    {
        this->name = nameIn;
        this->myHeader = new Heady(headerIn);
    }

    Relation(std::string newName, Heady *header, std::set<Tupler> newRows)
    {
        this->name = newName;
        this->myHeader = header;
        this->rows = newRows;
    }

    Relation columnSelect(int index, std::string value)
    {
        // int indexer = myHeader->findIndex(index);
        std::set<Tupler> tempRows;
        for (Tupler t : rows)
        {
            if (t.columnSelection(index, value))
            {
                tempRows.insert(t);
            }
        }

        Relation newRela = Relation(name, myHeader, tempRows);
        return newRela;
    }

    Relation doubleSelect(int indexOne, int indexTwo)
    {
        std::set<Tupler> tempRows;
        // int indexerOne = myHeader->findIndex(indexOne);
        // int indexerTwo = myHeader->findIndex(indexTwo);

        for (Tupler t : rows)
        {
            if (t.doubleSelection(indexOne, indexTwo))
            {
                tempRows.insert(t);
            }
        }

        Relation newRela = Relation(name, myHeader, tempRows);
        return newRela;
    }

    Relation project(std::vector<int> listofIndices)
    {
        // std::vector<int> indexList;
        // for (unsigned int i = 0; i < listofIndices.size(); i++)
        // {
        //     indexList.push_back(myHeader->findIndex(listofIndices[i]));
        // }

        std::set<Tupler> tempProjection;
        for (Tupler t : rows)
        {
            std::vector<std::string> tempVec = t.giveBack(listofIndices);
            tempProjection.insert(Tupler(tempVec));
        }

        Heady *newHead = new Heady(myHeader->getNewHeads(listofIndices));

        Relation newRela = Relation(name, newHead, tempProjection);
        return newRela;
    }

    Relation rename(std::vector<std::string> listofAttributes)
    {
        Heady *newRename = new Heady(listofAttributes);
        Relation newRela = Relation(name, newRename, rows);
        return newRela;
    }

    bool unioner(Relation newRulesRelation)
    {
        // Change input parameters to be two relations.
        
        bool addBool = false;
        // std::cout << rows.size() << std::endl;
        // std::cout << "^Before  V After" << std::endl;

        for (Tupler t : newRulesRelation.rows)
        {
            // rows.insert(t);
            if(rows.insert(t).second)
            {
                // Print out tuple??
                addBool = true;
                std::cout << "  ";
                std::string tupString;
                for (unsigned int i = 0; i < t.toString().size(); i++)
                {
                    tupString = tupString + myHeader->getValue(i) + "=" + t.toString()[i] + ", ";
                }
                tupString.pop_back();
                tupString.pop_back();
                std::cout << tupString;
                std::cout << std::endl;
            }
        }

        // std::cout << rows.size() << std::endl;
        // std::cout << name << std::endl;
        return addBool;

        
    }

    Relation joiner(Relation newRela)
    {

        // std::cout << "TODO: make the join function" << std::endl;
        // Combine headers
        Heady *newHeader = myHeader->combineHeaders(newRela.getHeader());
        // bool is joinable

        // Get the index of the common columns
        std::vector<std::string> headerOne = myHeader->getHeader();
        Heady *tempHead = newRela.getHeader();
        std::vector<std::string> headerTwo = tempHead->getHeader();
        std::vector<int> indexToCheck1;
        std::vector<int> indexToCheck2; 

        for (unsigned int i = 0; i < headerOne.size(); i++)
        {
            for (unsigned int j = 0; j < headerTwo.size(); j++)
            {
                if (headerOne[i] == headerTwo[j])
                {
                    indexToCheck1.push_back(i);
                    indexToCheck2.push_back(j);
                }
            }
        }

        // Get the tuples from relations
        std::set<Tupler> newRows = newRela.rows;
        std::set<Tupler> joinedRows;
        // Iterate through the tuples
        for (Tupler t: rows)
        {
            for (Tupler y : newRows) 
            {
                // hmmm join empty with valid?
                // TODO: cartesian product case
                if (isJoinable(t, y, indexToCheck1, indexToCheck2))
                {
                    joinedRows.insert(combineTuples(t, y, indexToCheck2));
                }
            }
        }

        
        Relation megaRela = Relation(name, newHeader, joinedRows);
        return megaRela;
    }

    bool isJoinable(Tupler row1, Tupler row2, std::vector<int> index1, std::vector<int> index2)
    {
        std::vector<std::string> strings1;
        std::vector<std::string> strings2;
        if (index1.size() == 0)
        {
            return true;
        }

        for (unsigned int i = 0; i < index1.size(); i++)
        {
            strings1.push_back(row1.getVal(index1[i]));
        }

        for (unsigned int i = 0; i < index2.size(); i++)
        {
            strings2.push_back(row2.getVal(index2[i]));
        }

        if (strings1 == strings2)
        {
            return true;
        }
        else 
        {
            return false;
        }
    }

    Tupler combineTuples(Tupler t1, Tupler t2, std::vector<int> index2)
    {
        // Load the entries of the tuples into a vector.
        std::vector<std::string> vec1 = t1.getAll();
        std::vector<std::string> vec2 = t2.getAll();

        // std::vector<std::string> sortedVec;

        // std::cout << "vector of first tuple" << std::endl;
        // for (unsigned int i = 0; i < vec1.size(); i++)
        // {
        //     std::cout << vec1[i];
        // }
        // std::cout << " " << std::endl;
        

        //  std::cout << "vector of indices" << std::endl;
        // for (unsigned int i = 0; i < index2.size(); i++)
        // {
        //     std::cout << index2[i];
        // }
        // std::cout << " " << std::endl;
        

        //  std::cout << "vector of second tuple" << std::endl;
        // for (unsigned int i = 0; i < vec2.size(); i++)
        // {
        //     std::cout << vec2[i];
        // }
        // std::cout << " " << std::endl;
        


        // // Brute force approach
        // int counter = 0;
        // for (unsigned int i = 0; i < index2.size(); i++)
        // {     
        //     vec2.erase(vec2.begin() + (index2[i]-counter));
        //     counter++;
        // }
        
        // std::cout << "vector of second tuple after erase" << std::endl;
        // for (unsigned int i = 0; i < vec2.size(); i++)
        // {
        //     std::cout << vec2[i];
        // }
        // std::cout << " " << std::endl;
        

        // for (unsigned int i = 0; i < vec2.size(); i++)
        // {
        //     vec1.push_back(vec2[i]);
        // }

        


















        
        for (unsigned int i = 0; i < vec2.size(); i++)
        {
            bool addable = true;
            for (unsigned int j = 0; j < index2.size(); j++)
            {
                if (index2[j] == (int) i)
                {
                    addable = false;
                }
            }
            if (addable)
            {
                vec1.push_back(vec2[i]);
            }
        }



        // std::cout << "vector of res tuple" << std::endl;
        // for (unsigned int i = 0; i < vec1.size(); i++)
        // {
        //     std::cout << vec1[i];
        // }
        // std::cout << " " << std::endl;

















        

        Tupler res = Tupler(vec1);
        return res;
    }

    void addRows(std::vector<std::string> inParams)
    {
        // Somehow we have to get this into order?
        Tupler tempRow = Tupler(inParams);
        rows.insert(tempRow);
        // std::cout << "Current Rows" << rows.size() << std::endl;

        // hmmmm commented this out but....
        // for (Tupler t : rows)
        // {
        //     std::vector<std::string> toPrint = t.toString();
        //     // for (unsigned int i = 0; i < toPrint.size(); i++)
        //     // {
        //     //     std::cout << toPrint[i] << std::endl;
        //     // }
        // }
        // // std::cout << "End of rows" << std::endl;
    }

    std::vector<std::string> toString()
    {
        std::cout << "This is the name" << name << std::endl;
        std::vector<std::string> res;
        std::vector<std::string> headBoi = myHeader->getHeader();
        for (unsigned int i = 0; i < headBoi.size(); i++)
        {
            std::cout << headBoi[i];
        }
        std::cout << std::endl;
        for (Tupler t : rows)
        {
            std::vector<std::string> aRow = t.toString();
            // if (aRow.size() != myHeader->getSize())
            // {
            //     std::cout << "Mismatched header and tuple!" << std::endl;
            // }

            std::string tempString = "";
            for (unsigned int i = 0; i < aRow.size(); i++)
            {
                tempString.append(myHeader->getValue(i));
                tempString.append("='");
                tempString.append(aRow[i]);
                tempString.append("',");
                // std::cout << "Here is the row: " << tempString;
                // std::cout << "In relation toString()!" << std::endl;
            }
            res.push_back(tempString);
        }
        return res;
    }

    Heady *getHeader()
    {
        Heady *copyHead = new Heady(myHeader->getHeader());
        return copyHead;
    }

    void setHeader(Heady *newHeader)
    {
        myHeader = newHeader;
        return;
    }

    std::set<Tupler> getRows()
    {
        return rows;
    }

    void printRes()
    {
        if (rows.size() > 0)
        {
            std::cout << "Yes(" << rows.size() << ")" << std::endl;
            std::vector<std::string> printHead = myHeader->getHeader();

            for (Tupler t : rows)
            {
                // std::cout << "Are we getting here?" << std::endl;
                std::string resLine = "  ";
                for (unsigned int i = 0; i < printHead.size(); i++)
                {
                    resLine.append(printHead[i]);
                    resLine.append("=");
                    resLine.append(t.getVal(i));
                    resLine.append(", ");
                }
                // std::cout << printHead[printHead.size() -  1] << "=" << t.getVal(printHead.size() - 1) << std::endl;
                resLine.pop_back();
                resLine.pop_back();
                if (resLine.size() > 2)
                {
                    std::cout << resLine << std::endl;
                }
            }
        }
        else
        {
            std::cout << "No" << std::endl;
        }
    }
};

#endif
