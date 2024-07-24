
#ifndef CGI_HPP
# define CGI_HPP

#include "Webserv.hpp"

class Server;
class ServerManager;
class HttpRequest;
class HttpResponse;
class Location;

class Cgi
{
private:
    char*       cgiFile;
    char**      env;
    char*       pass;
    char*       postBody;
    int         contentLen;
    std::string cgiPass;

    ServerManager*      manager;
    int                 pipeRead;
    int                 pipeWrite;
    std::vector<char>   cgiInput;
    std::vector<char>   cgiOutput;
    pid_t               childPid;
    bool                finishReading;
    size_t              appendBytes;

public:
    Cgi();
    Cgi(HttpRequest& req, Location& loc, Server& ser, ServerManager& sManager);
    ~Cgi();

    //Getters
    std::string getCgiPass() const {return cgiPass; }
    char*       getCgiFile() const {return cgiFile; }
    char*       getPostBody() const {return postBody; }
    char**      getEnv() const {return env; }
    int         getContentLen() const {return contentLen; }
	int         getPipeRead() const { return pipeRead; }
	int         getPipeWrite() const { return pipeWrite; }
	int         getFinishReading() const { return finishReading; }
    std::vector<char> getCgiOutput() const {return cgiOutput; }
    pid_t       getChildPid() const { return childPid; }
    
    //Setters
    void    setCgiFile(std::string s);
    void    setPostBody(HttpRequest& req);
    void    setContentLen(HttpRequest& req);
    void    setCgiEnv(HttpRequest& req, Location& loc, Server& ser);
    void    setCgiInput(std::string postBody);

    std::vector<char>    execCgi();
    void    childDup(int ToCgi[2], int FromCgi[2]);
    void    initParentPipe(int ToCgi[2], int FromCgi[2]);
    void    writeToCgi();
    void    readFromCgi();
    void    execCGI();
    void    putRequestIntoCgiInput(const std::string rawRequest);
    bool	isRunningCgi();
    void    rmPipesFromPollfd();

};

#endif
