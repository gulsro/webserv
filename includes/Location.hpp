
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Webserv.hpp"

class Location;
typedef void (Location::*setFunc2) (std::string&, int);

class Location
{
private:
    Server* server;
    std::string path;
    std::string root;
    unsigned long long maxBodySize;
    bool autoindex;
    std::string index;
    std::string redirect;
    std::map<int, std::string> errorPage;
    std::map<std::string, int> methods;
    std::string cgiPass;

    setFunc2 func[9] {&Location::setPath, &Location::setRoot, &Location::setMaxBodySize, &Location::setAutoindex,\
    &Location::setIndex, &Location::setRedirect, &Location::setMethods, &Location::setErrorPage, &Location::setCgiPass};

public:
    Location();
    Location(Server* s);
    ~Location();
    Location(const Location& a);
    Location& operator=(const Location a);

    std::string getPath() const;
    std::string getRoot() const;
    unsigned long long getMaxBodySize() const;
    bool getAutoindex() const;
    std::string getIndex() const;
    std::string getRedirect() const;
    std::map<int, std::string> getErrorPage() const;
    std::map<std::string, int> getMethods() const;
    std::string getCgiPass() const;

    void setPath(std::string& cont, int key);
    void setRoot(std::string& cont, int key);
    void setMaxBodySize(std::string& cont, int key);
    void setAutoindex(std::string& cont, int key);
    void setIndex(std::string& cont, int key);
    void setRedirect(std::string& cont, int key);
    void setMethods(std::string& cont, int key);
    void setErrorPage(std::string& cont, int key);
    void setCgiPass(std::string& cont, int key);

    void setLocationVar(std::stringstream& iss);
    void checkLocationVar();
};

std::ostream& operator<<(std::ostream& out, const Location& location);


#endif