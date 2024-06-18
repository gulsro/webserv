
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "utils.hpp"


class Location
{
private:
    // Server* server;
    std::string path;
    std::string root;
    unsigned long maxBodySize;
    bool autoindex;
    std::string index;
    std::string redirect;
    std::map<int, std::string> errorPage;
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
        const std::map<std::string, int> getMethods() { return methods; }

};



#endif