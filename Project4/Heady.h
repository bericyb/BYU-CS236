#ifndef Header_H
#define Header_H
#include <vector>
#include <string>


class Heady
{
private:
    std::vector<std::string> attributes;
    unsigned int size = 0;

public:
    Heady(std::vector<std::string> attributesIn)
    {
        this->attributes = attributesIn;
        // std::cout << "Creating new header!" << std::endl;
        // for (unsigned int i = 0; i < attributesIn.size(); i++)
        // {
        //     std::cout << attributes[i] << std::endl;
        // }
        this->size = attributesIn.size();
    }

    unsigned int getSize()
    {
        return size;
    }

    std::string getValue(int index)
    {
        return attributes[index];
    }

    std::vector<std::string> getNewHeads(std::vector<int> indices)
    {
        std::vector<std::string> res;
        for (unsigned i = 0; i < indices.size(); i++)
        {
            res.push_back(attributes[indices[i]]);
        }
        
        return res;
    }

    std::vector<std::string> getHeader()
    {
        return attributes;
    }

    Heady* combineHeaders(Heady* merger)
    {
        std::set<std::string> temp (attributes.begin(), attributes.end());
        std::vector<std::string> vec = attributes;
        std::vector<std::string> toMerge = merger->getHeader();
        for (unsigned int i = 0; i < toMerge.size(); i++)
        {
            if (temp.find(toMerge[i]) == temp.end())
            {
                vec.push_back(toMerge[i]);
            }
        }
        return new Heady(vec);
    }
};



#endif 