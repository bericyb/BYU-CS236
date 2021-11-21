#ifndef Parser_H
#define Parser_H
#include <iostream>
#include "Token.h"
#include "Rule.h"
#include "Parameter.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Scheme.h"
#include "Fact.h"
#include "Query.h"
#include "Domain.h"
#include <vector>
#include <string>
#include <set>

class Parser
{
private:
    std::vector<Token *> inTokens;
    int index = 0;
    int progIndex = -1;
    std::string state = "start";
    DatalogProgram *myProgram = new DatalogProgram();
    std::set<std::string> myDomain;
public:
    Parser(std::vector<Token *> input)
    {
        inTokens = input;
    }
    std::vector<std::string> Run()
    // datalogProgram -> SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON rulesList QUERIES COLON query queryList EOF
    {
        try
        {
            
            matcher("SCHEMES");
            matcher("COLON");
            state = "schemes";
            schemer();
            schemeLister();
            matcher("FACTS");
            matcher("COLON");
            state = "Facts";
            factLister();
            myProgram->addDomain(myDomain);
            matcher("RULES");
            matcher("COLON");
            state = "Rules";
            ruleLister();
            matcher("QUERIES");
            matcher("COLON");
            state = "Queries";
            queryer();
            queryLister();
            matcher("EOF");
            state = "End";
            
            return myProgram->toString();
        }
        catch (int e)
        {
            std::cout << "  " << inTokens[e]->toString() << std::endl;
            // std::cout << "Huh?" << std::endl;
            std::vector<std::string> theEmptyString;
            return theEmptyString;
        }
    }
    Token* matcher(std::string name)
    // Checks for a terminal using a string!
    {
        if (name == inTokens[index]->GetType())
        {
            index++;
            return inTokens[index - 1];
        }
        else
        {
            std::cout << "Failure!\n";
            throw index;
        }
    }

    void schemer()
    // scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN
    {
        state = "Schemer";
        if (inTokens[index]->GetType() == "FACTS")
        {
            // index++;
            // std::cout << myProgram->getSchemepred().size() << "This is the size of schemes" << std::endl;
            if (myProgram->getSchemepred().size() == 0)
            {
                std::cout << "Failure!\n";
                throw index;
            }
            return;
        }
        std::vector<Parameter*> temp;
        std::vector<Parameter*> list;
        std::string name = matcher("ID")->GetRaw();
        
        matcher("LEFT_PAREN");
        Parameter *first = new PlainParameter(matcher("ID")->GetRaw());
        list.push_back(first);

        temp = idLister();

        list.insert(list.end(), temp.begin(), temp.end()); 

        Predicate *preddy = new Predicate(name);
        preddy->addRaw(list);
        myProgram->addSchemepred(preddy);
    }

    std::vector<Parameter*> idLister()
    // idList -> COMMA ID idList | lambda
    {   
        std::vector<Parameter*> parameters;
        std::vector<Parameter*> temp;
        state = "IDlister";
        if (inTokens[index]->GetType() == "RIGHT_PAREN")
        {
            index++;
            return {};
        }
        matcher("COMMA");
        parameters.push_back(new PlainParameter(matcher("ID")->GetRaw()));
        temp = idLister();
        parameters.insert(parameters.end(), temp.begin(), temp.end());
        return parameters;
    }

    void schemeLister()
    // schemeList -> scheme schemeList | lambda
    {
        state = "SchemeLister";
        if (inTokens[index]->GetType() == "FACTS")
        {
            // index++;
            // std::cout << myProgram->getSchemepred().size() << "This is the size of schemes" << std::endl;
            if (myProgram->getSchemepred().size() == 0)
            {
                std::cout << "Failure!\n";
                throw index;
            }
            return;
        }
        schemer();
        schemeLister();
    }

    void factLister()
    //  factList -> fact factList | lambda
    {
        state = "factLister";
        if (inTokens[index]->GetType() == "RULES")
        {
            // index++;
            return;
        }
        facter();
        factLister();
    }

    void facter()
    // fact -> ID LEFT_PAREN ID idList RIGHT_PAREN
    {
        state = "facter";
        std::vector<Parameter*> temp;
        std::vector<Parameter*> list;
        std::string domainer;
        std::string name = matcher("ID")->GetRaw();
        matcher("LEFT_PAREN");
        domainer = matcher("STRING")->GetRaw();
        myDomain.insert(domainer);
        list.push_back(new PlainParameter(domainer));
        temp = stringLister();
        matcher("PERIOD");

        list.insert(list.end(), temp.begin(), temp.end()); 


        Predicate *preddy = new Predicate(name);
        preddy->addRaw(list);
        myProgram->addFactpred(preddy);
    }

    std::vector<Parameter*> stringLister()
    // stringList -> COMMA STRING stringList | lambda
    {
        state = "String Lister";
        std::vector<Parameter*> list;
        std::vector<Parameter*> temp;
        std::string domainer;
        if (inTokens[index]->GetType() == "RIGHT_PAREN")
        {
            index++;
            return list;
        }
        matcher("COMMA");
        domainer = matcher("STRING")->GetRaw();
        myDomain.insert(domainer);
        list.push_back(new PlainParameter(domainer));
        temp = stringLister();
        list.insert(list.end(), temp.begin(), temp.end());
        return list;
    }

    void ruleLister()
    // ruleList -> rule ruleList | lambda
    {
        state  = "Ruler lister ";
        if (inTokens[index]->GetType() == "QUERIES")
        {
            // index++;
            return;
        }
        ruler();
        ruleLister();
    }

    void ruler()
    // rule -> headPredicate COLON_DASH predicate predicateList PERIOD
    {
        state = "rule";
        std::vector<Predicate*> preddies;
        std::vector<Predicate*> temp;

        Rule *first = new Rule(headPredicater());
        matcher("COLON_DASH");
        Predicate *second = new Predicate();
        predicater(second);
        preddies.push_back(second);
        temp = predicateLister();

        preddies.insert(preddies.end(), temp.begin(), temp.end());

        first->addPredicate(preddies);
        myProgram->addRules(first);
        matcher("PERIOD");
    }

    Predicate* headPredicater()
    // headPredicate -> ID LEFT_PAREN ID idList RIGHT_PAREN
    {
        state = "Head predicate";
        std::vector<Parameter*> temp;
        std::vector<Parameter*> list;
        Predicate *first = new Predicate(matcher("ID")->GetRaw());
        matcher("LEFT_PAREN");
        list.push_back(new PlainParameter(matcher("ID")->GetRaw()));
        temp = idLister();

        list.insert(list.end(), temp.begin(), temp.end());
    
        first->addRaw(list);
        return first;
    }

    void predicater(Predicate* predder)
    // predicate -> ID LEFT_PAREN parameter parameterList RIGHT_PAREN
    {
        state = "Predicate";
        std::vector<Parameter*> list;
        std::vector<Parameter*> temp;
        predder->setName(matcher("ID")->GetRaw());
        matcher("LEFT_PAREN");
        list.push_back(parameterer());
        // Not sure what to do here!
        temp = parameterLister();

        list.insert(list.end(), temp.begin(), temp.end());

        predder->addRaw(list);
    }

    std::vector<Predicate*> predicateLister()
    // predicateList -> COMMA predicate predicateList | lambda
    {
        state = "predicateList";
        std::vector<Predicate*> predsters;
        std::vector<Predicate*> temp;
        if (inTokens[index]->GetType() == "PERIOD")
        {
            // index++;
            return {};
        }
        matcher("COMMA");
        Predicate* newGuy = new Predicate();
        predicater(newGuy);
        predsters.push_back(newGuy);
        temp = predicateLister();
        
        predsters.insert(predsters.end(), temp.begin(), temp.end());

        return predsters;
    }

    std::vector<Parameter*> parameterLister()
    // parameterList -> COMMA parameter parameterList | lambda
    {
        state = "parameter list";
        std::vector<Parameter*> chickParms;
        std::vector<Parameter*> temp;
        if (inTokens[index]->GetType() == "RIGHT_PAREN")
        {
            index++;
            return {};
        }
        matcher("COMMA");
        chickParms.push_back(parameterer());
        temp = parameterLister();
        chickParms.insert(chickParms.end(), temp.begin(), temp.end());
        return chickParms;
    }

    Parameter* parameterer()
    // parameter -> STRING | ID | expression
    {
        state = "Parameter";
        
        if (inTokens[index]->GetType() == "STRING")
        {
            Parameter* para = new PlainParameter(inTokens[index]->GetRaw());
            
            index++;
            return para;
        }
        else if (inTokens[index]->GetType() == "ID")
        {
            Parameter* para = new PlainParameter(inTokens[index]->GetRaw());
            index++;
            return para;
        }
        else
        {
            return expressioner();
        }
    }

    Parameter* expressioner()
    // expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN
    {   
        std::vector<Parameter*> express;
        state = "Expression";
        matcher("LEFT_PAREN");
        express.push_back(parameterer());
        express.push_back(operatorer());
        express.push_back(parameterer());
        matcher("RIGHT_PAREN");
        Parameter *myExpress = new Expression(express);
        return myExpress;
    }

    Parameter* operatorer()
    // operator -> ADD | MULTIPLY
    {
        
        state = "Operator";
        if (inTokens[index]->GetType() == "ADD")
        {
            Parameter* para = new PlainParameter(inTokens[index]->GetRaw());
            index++;
            return para;
        }
        else if (inTokens[index]->GetType() == "MULTIPLY")
        {
            Parameter* para = new PlainParameter(inTokens[index]->GetRaw());
            index++;
            return para;
        }
        else
        {
            matcher("ADD or MULIPLY");
            Parameter* para = new PlainParameter("FAIL!");
            return para;
        }
        
    }

    void queryer()
    // query -> predicate Q_MARK
    {
        state = "query";
        Predicate *preddy = new Predicate();
        predicater(preddy);
        myProgram->addQuerypred(preddy);
        matcher("Q_MARK");
    }

    void queryLister()
    // queryList -> query queryList | lambda
    {
        state = "Query list";
        if (inTokens[index]->GetType() == "EOF")
        {
            // index++;
            return;
        }
        else
        {
            queryer();
            queryLister();
        }
    }
};

#endif