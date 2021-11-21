#ifndef Tuple_H
#define Tuple_h
#include <vector>
#include <string>

class Tupler
{
private:
    std::vector<std::string> values;
    bool mergable = false;
public:
    Tupler(std::vector<std::string> inVal)
    {
        this->values = inVal;
    }
    
    std::vector<std::string> toString()
    {
        return values;
    }

    bool operator<(const Tupler &toCheck) const
    {
        // TODO: compare this Tuple to other Tuple????
        if (values < toCheck.values)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool columnSelection(int index, std::string keyValue)
    {
        if (values[index] == keyValue)
        {
            return true;
        }
        else {
            return false;
        }
    }

    bool doubleSelection(int indexOne, int indexTwo)
    {
        if (values[indexOne] == values[indexTwo])
        {
            return true;
        }
        else 
        {
            return false;
        }
    }

    std::vector<std::string> giveBack(std::vector<int> indexList)
    {
        std::vector<std::string> temp;
        for (unsigned int i = 0; i < indexList.size(); i++)
        {
            temp.push_back(values[indexList[i]]);
        }
        return temp;
    }

    std::string getVal(int index)
    {
        return values[index];
    }

    void setMergable(bool cond)
    {
        mergable = cond;
        return;
    }

    std::vector<std::string> getAll()
    {
        return values;
    }
};

#endif