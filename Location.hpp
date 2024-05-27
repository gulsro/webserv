
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <iostream>


class Location
{
private:
    std::string path;
    std::string root;
    bool autoindex;
    std::string index;
    std::string redirect;
    std::string alias;
    std::string error_page;

    std::vector<bool> methods; //GET, POST DELETE

    std::string cgi_pass;




public:
    Location();
    ~Location();
    Location(Location& a);
    Location& operator=(const Location a);


};

#endif