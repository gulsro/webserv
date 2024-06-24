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
    Cgi(HttpResponse& response);
    ~Cgi();
    Cgi(Cgi& a);
    Cgi& operator=(const Cgi a);

    char **initCgiEnv(HttpResponse& response);
    void    execCgi();
};

#endif