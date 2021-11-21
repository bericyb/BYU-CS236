#ifndef Rule_H
#define Rule_H
#include "Predicate.h"

class Rule 
{  
private:   
    std::vector<Predicate*> preds;
    

public:
    Rule(Predicate* input)
    {
        preds.push_back(input);
    }
    void addPredicate(std::vector<Predicate*> input)
    {
        preds.insert(preds.end(), input.begin(), input.end());
    }
    std::string ToString()
    {
        std::string final;
        // final.append("  ");
        final.append(preds[0]->ToString());
        final.append(" :- ");
        for (unsigned int i = 1; i < preds.size(); i++)
        {
            final.append(preds[i]->ToString());
            final.append(",");
        }
        final.pop_back();
        // final.append(".");
        return final;
    }
};

#endif