#include "../include/Location.hpp"

Location::Location() : path(""), root(""), autoindex(false), index(""), redirect(""), cgiPass("")
{
    // std::cout << "Location is constructed" << std::endl;
    methods["GET"] = 1;
    methods["POST"] = 0;
    methods["DELETE"] = 0;

}

Location::~Location(){
    // std::cout << "Location is destructed" << std::endl;
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

//--------------Getters-------------------

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

std::string Location::getCgiPass() const{
    return this->cgiPass;
}

//--------------Setters-------------------

void Location::setPath(std::string& cont, int key){
    if (!getPath().empty())
        return ;
    while(std::isspace(cont[key]))
        key--;
    this->path = cont.substr(0, key);
}

void Location::setRoot(std::string& cont, int key){
    if (!getRoot().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->root = cont.substr(key, cont.find('\n') - key);
}

void Location::setAutoindex(std::string& cont, int key){
    if (getAutoindex())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->autoindex = true;
}

void Location::setIndex(std::string& cont, int key){
    if (!getIndex().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->index = cont.substr(key, cont.find('\n') - key);
}

void Location::setRedirect(std::string& cont, int key){
    if (!getRedirect().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->redirect = cont.substr(key, cont.find('\n') - key);
}

void Location::setMethods(std::string& cont, int key){
    key = 0;
    if (cont.find("POST") != std::string::npos)
        methods["POST"] = 1;
    if (cont.find("DELETE") != std::string::npos)
        methods["DELETE"] = 1;
}


void Location::setErrorPage(std::string& cont, int key){
    while(std::isspace(cont[key]))
        key++;
}

void Location::setCgiPass(std::string& cont, int key){
    if (!getCgiPass().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->cgiPass = cont.substr(key, cont.find('\n') - key);
}

//--------------Functions-------------------

void Location::setLocationVar(std::stringstream& iss){

    std::size_t key;
    std::string line;
    std::string parameter[8] = {"{", "root", "autoindex", "index", "return", "methods", "error_page", "cgi_pass"};
     while (std::getline(iss, line, '\n')){
        for (int i = 0; i < 8; i++){
            key = line.find(parameter[i]);
            if(key != std::string::npos){
                if (i == 0)
                    key -= 1;
                (this->*func[i])(line, key + parameter[i].size());
            }
        }
    }
    // std::cout << RED << *this << RES << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Location& location)
{
    out << "path: " << location.getPath()<< std::endl;
    out << "root: " << location.getRoot()<< std::endl;
    out << "index: " << location.getIndex()<< std::endl;
    out << "autoindex: " << location.getAutoindex()<< std::endl;
    out << "redirect: " << location.getRedirect()<< std::endl;
    out << "methods: ";
    if (location.getMethods()["GET"] == 1)
        out << "GET ";
    if(location.getMethods()["POST"] == 1)
        out << "POST ";
    if (location.getMethods()["DELETE"] == 1)
        out << "DELETE";
    out << std::endl<< "cgi_pass: " << location.getCgiPass() << std::endl;
    out << std::endl;
    return out;
}