#ifndef DatalogProgram_H
#define DatalogProgram_H
#include "Predicate.h"
#include "Rule.h"
#include <set>


class DatalogProgram 
{
private:
    std::vector<Predicate*> Schemepreds;
    std::vector<Predicate*> Factpreds;
    std::vector<Predicate*> Querypreds;
    std::vector<Rule*> rules;
    std::set<std::string> domain;
public:
    std::vector<std::string> toString()
    {
        std::vector<std::string> final;
        int sizer = 0;
        sizer = Schemepreds.size();
        final.push_back("Schemes(" + std::to_string(sizer) + "):");
        for (unsigned int i = 0; i < Schemepreds.size(); i++)
        {
            final.push_back("  " + Schemepreds[i]->ToString());
        }
        sizer = Factpreds.size();
        final.push_back("Facts(" + std::to_string(sizer) + "):");
        for (unsigned int j = 0; j < Factpreds.size(); j++)
        {
            final.push_back("  " + Factpreds[j]->ToString() + ".");
        }
        sizer = rules.size();
        final.push_back("Rules(" + std::to_string(sizer) + "):");
        for (unsigned int k = 0; k < rules.size(); k++)
        {
            final.push_back("  " + rules[k]->ToString() + ".");
        }
        sizer = Querypreds.size();
        final.push_back("Queries(" + std::to_string(sizer) + "):");
        for (unsigned int l = 0; l < Querypreds.size(); l++) 
        {
            final.push_back("  " + Querypreds[l]->ToString() + "?");
        }
        sizer = domain.size();
        final.push_back("Domain(" + std::to_string(sizer) + "):");
        for (std::set<std::string>::iterator it = domain.begin(); it != domain.end(); it++) 
        {
            final.push_back("  " + *it);
        }
        return final;
    }
    void addSchemepred(Predicate* pred)
    {
        Schemepreds.push_back(pred);
    }
    void addFactpred(Predicate* pred)
    {
        Factpreds.push_back(pred);
    }
    void addQuerypred(Predicate* pred)
    {
        Querypreds.push_back(pred);
    }
    void addRules(Rule* pred)
    {
        rules.push_back(pred);
    }
    void addDomain(std::set<std::string> input)
    {
        domain = input;
    }
    std::vector<Predicate*> getSchemepred()
    {
        return Schemepreds;
    }
    std::vector<Predicate*> getFactpred()
    {
        return Factpreds;
    }
    std::vector<Predicate*> getQuerypred()
    {
        return Querypreds;
    }
    std::vector<Rule*> getRules()
    {
        return rules;
    }
};


#endif