
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Webserv.hpp"


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
    Location();
    ~Location();
    Location(const Location& a);
    Location& operator=(const Location a);


};

#endif