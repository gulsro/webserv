#include "Location.hpp"

Location::Location(){

}

Location::~Location(){

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
    this->alias = a.alias;
    this->errorPage = a.errorPage;
    this->methods = a.methods;
    this->cgiPass = a.cgiPass;
    return (*this);
}