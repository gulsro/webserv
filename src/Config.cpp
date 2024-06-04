#include "../include/Config.hpp"

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

bool Config::isBlank(std::ifstream& file){
    std::istreambuf_iterator<char> begin(file), end;

    while(begin != end){
        if (!std::isspace(*begin))
            return (false);
        begin++;
    }
    return (true);
}

//read from file and remove leading whitespaces, put it in one string <content>
void Config::readConfig(std::ifstream& file){
    std::string line;
    std::stringstream buf;

    while (std::getline(file >> std::ws, line))
        buf << line << std::endl;
    this->content = buf.str();
    // std::cout << MAG << content << RES << std::endl;
}

void Config::parseConfig(){
    std::ifstream file;
    // std::cout << "config file is: " << configFile << std::endl;
    checkConfig(file);
    readConfig(file);
    file.close();
    // cntServer();
    splitServer();
}

void Config::checkConfig(std::ifstream& file){
    file.open(configFile, std::ios::in | std::ios::out);
    if (!file)
        throw std::runtime_error("unable to open file");
    if (!std::filesystem::file_size(configFile))
    {
        file.close();
        throw std::runtime_error("empty file");
    }
    if (isBlank(file))
    {
        file.close();
        throw std::runtime_error("file has only whitespaces");
    }
}

//still need to check if server section is properly closed or not etc.
void Config::cntServer(){
    std::size_t found = this->content.find("server {");
    while (found != std::string::npos){
        this->nbServer++;
        found = this->content.find("server {", found + 1, 8);
    }
    if (!this->nbServer)
        throw std::runtime_error("No server was found in config file");
    std::cout << "# of server is: " << nbServer << std:: endl;
}

//set outside of class
std::size_t findScopeBegin(std::string& content, size_t found){
    std::size_t begin = found + 1;
    while (std::isspace(content[begin]))
        begin++;
    if (content[begin] != '{')
        throw std::runtime_error("Server scope is wrong");
    return (begin + 1);
}

std::size_t findScopeEnd(std::string& content, size_t begin){
    std::size_t end = begin;
    int bracket = 1;
    while (bracket && content[end]){
        if (content[end] == '{')
            bracket++;
        if (content[end] == '}')
            bracket--;
        end++;
    }
    return(end - 1);
}

void Config::splitServer(){
    std::size_t found = this->content.find("server");
    while (found != std::string::npos){
        std::size_t begin = findScopeBegin(this->content, found + 6);
        std::size_t end = findScopeEnd(this->content, begin);
        this->serverCont.push_back(content.substr(begin, end - begin));
        this->nbServer++;
        begin = end + 1;
        found = this->content.find("server", begin, 6);
    }
    for (std::string s : serverCont)
        std::cout << BLU << s << RES << std::endl << std::endl;
    std::cout << "# of server is: " << nbServer << std:: endl;

}