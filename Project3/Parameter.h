#ifndef Parameter_H
#define Parameter_H

class Parameter
{
protected:
public:
    std::string parachute;
    Parameter()
    {
    }
    std::string ToString()
    {
        return parachute;
    }
};

class PlainParameter : public Parameter
{
private:

public:
    PlainParameter(std::string plain)
    {
        this->parachute = plain;
    }

};

class Expression : public Parameter
{
private:

public:
    Expression(std::vector<Parameter*> input)
    {
        parachute.append("(");
        for (unsigned i = 0; i < input.size(); i++) 
        {
            parachute.append((input[i]->parachute));
            // parachute.append(",");
        }
        // parachute.pop_back();
        parachute.append(")");
    }

};

#endif