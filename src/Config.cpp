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

bool Config::isBlank(std::fstream& file){
    if (file.peek() != EOF && !std::isspace(file.peek()))
        return (false);
    return (true);
}

//read from file and remove leading whitespaces, put it in one string <content>
void Config::readConfig(std::fstream& file){
    std::string line;
    std::stringstream buf;

    while (std::getline(file >> std::ws, line))
        buf << line << std::endl;
    this->content = buf.str();
    // std::cout << MAG << content << RES << std::endl;
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
    else if (this->isBlank(file))
    {
        file.close();
        throw std::runtime_error("file has only whitespaces");
    }
    readConfig(file);
    file.close();
}


/*

Config part:
5. erase comments and whitespaces in the beginning??
6. create another string vector for each server blocks + count nbServer

Server part:

*/