
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
    std::map<std::string, int> methods;
    std::string cgiPass;

    setFunc2 func[8] {&Location::setPath, &Location::setRoot, &Location::setMaxBodySize, &Location::setAutoindex,\
    &Location::setIndex, &Location::setRedirect, &Location::setMethods, &Location::setCgiPass};

public:
    Location();
    Location(Server* s);
    ~Location();
    Location(const Location& a);
    Location& operator=(const Location a);

    std::string getPath() const { return this->path; }
    std::string getRoot() const { return this->root; }
    unsigned long long getMaxBodySize() const { return this->maxBodySize; }
    bool getAutoindex() const { return this->autoindex; }
    std::string getIndex() const { return this->index; }
    std::string getRedirect() const { return this->redirect; }
    std::map<std::string, int> getMethods() const { return this->methods; }
    std::string getCgiPass() const { return this->cgiPass; }

    void setPath(std::string& cont, int key);
    void setRoot(std::string& cont, int key);
    void setMaxBodySize(std::string& cont, int key);
    void setAutoindex(std::string& cont, int key);
    void setIndex(std::string& cont, int key);
    void setRedirect(std::string& cont, int key);
    void setMethods(std::string& cont, int key);
    void setCgiPass(std::string& cont, int key);

    void setLocationVar(std::stringstream& iss);
    void checkLocationVar();
};

std::ostream& operator<<(std::ostream& out, const Location& location);


#endif