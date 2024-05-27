
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

    std::vector<bool> methods; //GET, POST DELETE

    std::string cgiPass;




public:
    Location();
    ~Location();
    Location(Location& a);
    Location& operator=(const Location a);


};

#endif