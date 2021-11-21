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

    void addRows(std::vector<std::string> inParams)
    {
        // Somehow we have to get this into order?
        Tupler tempRow = Tupler(inParams);
        rows.insert(tempRow);
        // std::cout << "Current Rows" << rows.size() << std::endl;
        for (Tupler t : rows)
        {
            std::vector<std::string> toPrint = t.toString();
            // for (unsigned int i = 0; i < toPrint.size(); i++)
            // {
            //     std::cout << toPrint[i] << std::endl;
            // }
        }
        // std::cout << "End of rows" << std::endl;
    }

    std::vector<std::string> toString()
    {
        std::vector<std::string> res;
        for (Tupler t : rows)
        {
            std::vector<std::string> aRow = t.toString();
            if (aRow.size() != myHeader->getSize())
            {
                std::cout << "Mismatched header and tuple!" << std::endl;
            }

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
                if (resLine.size() > 2) {
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
