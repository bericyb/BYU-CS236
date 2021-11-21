#ifndef Predicate_H
#define Predicate_H
#include "Parameter.h"

class Predicate
{
private:
    std::string name;
    std::vector<Parameter*> params;
public:
    Predicate()
    {
        this->name = "Placeholder";
    }
    Predicate(std::string newName)
    {

        this->name = newName;
    }
    void addRaw(std::vector<Parameter*> input)
    {
        params = input;
    }
    void setName(std::string input)
    {
        this->name = input;
    }
    std::string ToString() const
    {
        std::string final;
        // final.append("  ");
        final.append(name);
        final.append("(");
        for (unsigned int i = 0; i < params.size(); i++) 
        {
            final.append(params[i]->ToString());
            final.append(",");
        }
        final.pop_back();
        final.append(")");
        return final;
    }
    std::string getName() const
    {
        return name;
    }
    std::vector<std::string> getParam() const
    {
        std::vector<std::string> res;
        for (unsigned i = 0; i < params.size(); i++)
        {
            res.push_back(params[i]->ToString());
        }
        return res;
    }
};

#endif