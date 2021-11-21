#include "Database.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>
#include<fstream>

int main(int argc, char *argv[])
{
    std::string filename = argv[1];
    std::ifstream ifs(filename);

    // std::string content((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    // std::cout << content << std::endl;

    std::string str;
    std::vector<std::string> parsed;
    while (std::getline(ifs, str))
    {
        if (str.size() > 0){
            parsed.push_back(str);
        } 
    }

    Database *Database = new Database(parsed);
    result = Database->Run();

    return 0;
};