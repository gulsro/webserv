#ifndef CGI_HPP
# define CGI_HPP

#include "Webserv.hpp"

class Server;
class HttpRequest;
class HttpResponse;
class Location;

class Cgi
{
private:
    std::string cgiPass;
    char *cgiFile;
    char **env;
    
public:
    Cgi();
    Cgi(HttpRequest& req, Location& loc, Server& ser);
    ~Cgi();
    Cgi(Cgi& a);
    Cgi& operator=(const Cgi a);

    std::string getCgiPass() const {return cgiPass; };
    char* getCgiFile() const {return cgiFile; };
    char** getEnv() const {return env; };

    void   setCgiFile(std::string s);
    void setCgiEnv(HttpRequest& req, Location& loc, Server& ser);
    std::string    execCgi();
};

#endif