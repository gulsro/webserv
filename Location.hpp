
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "utils.hpp"


class Location
{
private:
    std::string path;
    std::string root;
    bool autoindex;
    std::string index;
    std::string redirect;
    std::string alias;
    std::string errorPage;

    std::map<std::string, int> methods;
    std::string cgiPass;

    public:
        Location() 
        { 
            this->path  = "/upload";
            this->root = "/root";
            this->autoindex =  false;
            this->index =  "index.html";
        }

        const std::string	getPath() const { return path; }
        const std::string   getRoot() const { return root; }
        const std::string   getIndex() const { return index; }
        bool                isAutoindex() { return autoindex; }

};



#endif