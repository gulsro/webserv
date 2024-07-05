#include "../includes/Cgi.hpp"

#define BUFFER_SIZE 1024
/*Because you won’t call the CGI directly, use the full path as PATH_INFO.
∗ Just remember that, for chunked request, your server needs to unchunk
it, the CGI will expect EOF as end of the body.
∗ Same things for the output of the CGI. If no content_length is returned
from the CGI, EOF will mark the end of the returned data.
∗ Your program should call the CGI with the file requested as first argument.
∗ The CGI should be run in the correct directory for relative path file access.
∗ Your server should work with one CGI (php-CGI, Python, and so forth)*/


Cgi::Cgi(){}

Cgi::Cgi(HttpRequest& req, Location& loc, Server& ser){
    cgiPass = loc.getCgiPass();
    std::cout << cgiPass << std::endl;
    setCgiFile();
    setCgiEnv(req, loc, ser);
}


Cgi::~Cgi(){}

Cgi::Cgi(Cgi& a){
    operator=(a);
}

Cgi& Cgi::operator=(const Cgi a){
    if (this == &a)
        return (*this);
    this->cgiPass = a.getCgiPass();
    this->cgiFile = a.getCgiFile();
    this->env = a.getEnv();
    return (*this);
}

void Cgi::setCgiFile(){
    cgiFile = nullptr;
    std::size_t found = cgiPass.rfind("/");

    if (found != std::string::npos){
        std::string tmp = "./" + cgiPass.substr(found + 1);
        cgiFile = new char[tmp.size() + 1];
        std::strcpy(cgiFile, tmp.c_str());
    }
}



// parse in tmp and copy it to char* env
void Cgi::setCgiEnv(HttpRequest& req, Location& loc, Server& ser){
    std::vector<std::string> tmp;
    std::vector<char *> CgiEnv;

    tmp.push_back("GATEWAY_INTERFACE=cgi/1.1");
    tmp.push_back("SERVER_NAME=" + ser.getHost()); //server hostname
    tmp.push_back("SERVER_SOFTWARE=webserv/1.0");
    tmp.push_back("SERVER_PROTOCOL=HTTP/1.1");
    tmp.push_back("SERVER_PORT=" + std::to_string(ser.getPort())); //server port
    tmp.push_back("REQUEST_METHOD=" + req.getMethod()); //request method
    tmp.push_back("SCRIPT_NAME="+ loc.getCgiPass()); //cgi pass
    tmp.push_back("DOCUMENT_ROOT=" + loc.getRoot()); //location getRoot()
    tmp.push_back("QUERY_STRING=" + req.getQueryString()); //getQuery
    if (req.getMethod() == "POST"){
        tmp.push_back("CONTENT_TYPE=" + req.getContentType()); // ex. text/html
        tmp.push_back("CONTENT_LENGTH=" + std::to_string(req.getContentLength()));
    }
    for (std::string s : tmp)
        CgiEnv.push_back(&s.front());
    this->env = CgiEnv.data();
}

//return http response msg or '\0' in case of internal error
std::string    Cgi::execCgi(){
    int pip[2];
    pid_t pid;

    std::cout << MAG << "CGI executed" << RES << std::endl;
    if (pipe(pip) < 0)
		perror("pipe failed"); //error
	pid = fork();
	if (pid < 0)
		perror("fork failed"); //error
	if (pid == 0){
        // close read end and write to pipe 
        //output of cgi script will be written in pipe
        char *argv[2] = {cgiFile, NULL};
        close(pip[0]); 
        std::cout << MAG << "child process: " << RES << std::endl;
        if (dup2(pip[1], STDOUT_FILENO) < 0) 
            perror("write pipe failed"); //error
        if (execve(cgiPass.c_str(), argv, env) < 0)
            exit(1);
    }
    int status;
    char buf[BUFFER_SIZE]; // is buffer_size defined in config?
    std::string body = "";
    ssize_t bytes = 1;
//close write end and read output from pipe
    close(pip[1]);
    if (waitpid(pid, &status, 0) < 0)
        perror("wait failed"); 

    if (WIFEXITED(status) == false &&(WEXITSTATUS(status)!= 0)) 
        return ("wait exit status failed");
    if (dup2(pip[0], STDIN_FILENO) < 0)
        return ("read pipe failed"); // error
    while (bytes > 0){
        std::memset(buf, '\0', BUFFER_SIZE - 1);
        bytes = read(pip[1], buf, BUFFER_SIZE);
        // if (bytes < 0)
            //no read;
        printf("%zd bytes read: %s ", bytes, buf);
        body = body + buf;
    }
    printf("\n");
    close(pip[0]);
    return ("HTTP/1.1 200 OK\r\n" + body);
}

/*
1. check if the file requested is located in the location directory with cgi pass 
    in HttpRequest::setReqLocation 

3. make a child process by fork() and open pipe

4. Execute CGI in child process.
    redirect stdout to write end of the pipe
    av[0] = cgi file name 
    av[1] = path to the cgi file
    av[2] = env or NULL (need to figure out)

env has parsed request and some more variables according to RFC3875

Meta-variables with names beginning with "HTTP_" contain values read 
from the client request header fields, if the protocol used is HTTP.
The HTTP header field name is converted to upper case, has all    
occurrences of "-" replaced with "_" and has "HTTP_" prepended to give the meta-variable name.

5. Pass requested data through pipe in Parent Process.
Parent, preads from 


if config file contains following:
location *.php {
methods GET POST
cgi_pass /usr/bin/php-cgi
return https:www.google.com
}

*.php is passed as the first parameter of execve() and /usr/bin/php-cgi will be used as the second parameter (third parameter is env)

So the CGI steps will be...


    Apache executes the CGI program, passing the parameters from the request to the CGI program in the environment (as environment variables).

    The CGI program gets the parameters from the environment, performs any required processing, and writes the web page on standard output.

    Apache receives the web page from the CGI program's standard output and transmits it to the web client (usually your web browser).


*/