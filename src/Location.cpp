#include "../include/Location.hpp"

Location::Location(){
    std::cout << "Location is constructed" << std::endl;
}

Location::~Location(){
    std::cout << "Location is destructed" << std::endl;
}

Location::Location(const Location& a){
    operator=(a);
}

Location& Location::operator=(const Location a){
    if (this == &a)
        return (*this);
    this->path = a.path;
    this->root = a.root;
    this->autoindex = a.autoindex;
    this->redirect = a.redirect;
    // this->alias = a.alias;
    this->errorPage = a.errorPage;
    this->methods = a.methods;
    this->cgiPass = a.cgiPass;
    return (*this);
}

void Location::setLocationVar(std::stringstream iss){

    // std::size_t key;
    // std::string line;
    // std::string parameter[4] = {"listen", "root", "index", "client_size"};
    //  while (std::getline(iss, line, '\n')){
    //     for (int i = 0; i < 4; i++){
    //         key = line.find(parameter[i]);
    //         if(key != std::string::npos)
    //             (this->*func[i])(line, key + parameter[i].size());
    //     }
    // }
    // std::cout << YEL << *this << RES << std::endl;
}


std::string Location::getPath() const{
    return this->path;
}

std::string Location::getRoot() const{
    return this->root;
}

bool Location::getAutoindex() const{
    return this->autoindex;
}

std::string Location::getIndex() const{
    return this->index;
}

std::string Location::getRedirect() const{
    return this->redirect;
}

std::map<std::string, int> Location::getMethods() const{
    return this->methods;
}



std::ostream& operator<<(std::ostream& out, const Location& location)
{
    out << "path: " << location.getPath()<< std::endl;
    out << "serverFd: " << location.getRoot()<< std::endl;
    out << "host: " << location.getAutoindex()<< std::endl;
    out << "root: " << location.getIndex()<< std::endl;
    out << "index: " << location.getRedirect()<< std::endl;
    out << "methods: ";
    if (location.getMethods()["GET"] == true)
        out << "GET ";
    if(location.getMethods()["POST"] == true)
        out << "POST ";
    if (location.getMethods()["DELETE"] == true)
        out << "DELETE";
    out << std::endl << std::endl;
    return out;
}