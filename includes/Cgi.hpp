#ifndef CGI_HPP
# define CGI_HPP

#include "Webserv.hpp"

class Server;

class Cgi
{
private:
    char **env;
    
public:
    Cgi();
    Cgi(HttpRequest& req, Location& loc, Server& ser);
    ~Cgi();
    Cgi(Cgi& a);
    Cgi& operator=(const Cgi a);

    char **initCgiEnv(HttpRequest& req, Location& loc, Server& ser);
    std::string    execCgi();
};

#endif