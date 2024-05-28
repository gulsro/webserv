#include "../include/Config.hpp"
// #include <filesystem>
// #include <iterator>
// #include <algorithm>

Config::Config(){
    this->configFile = "./configs/default.conf";
    this->nbServer = 0;
}

Config::~Config(){

}


void    Config::setConfigFile(std::string file)
{
    this->configFile = file;
}

bool Config::isWhitespace(std::fstream& file){
    if (file.peek() != EOF && !std::isspace(file.peek()))
        return (false);
    return (true);
}

void Config::eraseWhitespace(std::fstream& file){
    std::string line;
    while (std::getline(file >> std::ws, line))
    {
        // std::cout << line << std::endl;
        if (line.empty())
            std::cout << std::endl;
        // std::cout << line << std::endl;
        file << line << std::endl;
    }
}

void Config::checkConfig(){
    std::fstream file;
    std::cout << configFile << std::endl;
    file.open(configFile, std::ios::in | std::ios::out);
    if (!file)
        throw std::runtime_error("unable to open file");
    if (!std::filesystem::file_size(configFile))
    {
        file.close();
        throw std::runtime_error("empty file");
    }
    else if (this->isWhitespace(file))
    {
        file.close();
        throw std::runtime_error("file has only whitespaces");
    }
    eraseWhitespace(file); // this is not working. fnd ways to replace prev lines
    file.close();
}


/*

Config part:
5. erase comments and whitespaces in the beginning??
6. create another string vector for each server blocks + count nbServer

Server part:

*/