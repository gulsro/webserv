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

Cgi::Cgi(HttpRequest& req, Location& loc, Server& ser) : pass(NULL), postBody(NULL), contentLen(0)
{
    cgiPass = loc.getCgiPass();
    std::cout << loc.getRoot() << " aaaaaaaaaaaaaaaaaaaaaaand " << req.getURI() << std::endl;
    setCgiFile("."+loc.getRoot()+req.getURI());
    setCgiEnv(req, loc, ser);
    setPostBody(req);
    setContentLen(req);
}

Cgi::~Cgi(){
    #ifdef STRUCTOR
		std::cout << GREY << "Cgi : Default destructor called" << DEFAULT << std::endl; 
	#endif
    delete[] this->cgiFile;
    for (int i = 0; env[i] != NULL; ++i)
        delete[] env[i];
    delete[] this->env;
    delete[] this->pass;
}

// Cgi::Cgi(Cgi& a){
//     operator=(a);
// }

// Cgi& Cgi::operator=(const Cgi a){
//     if (this == &a)
//         return (*this);
//     this->cgiPass = a.getCgiPass();
//     this->cgiFile = a.getCgiFile();
//     this->env = a.getEnv();
//     return (*this);
// }

// // check if this is valid for bth GET and POST
// void Cgi::setCgiFile(std::string s){
//     cgiFile = new char[s.size() + 1];
//     std::strcpy(cgiFile, s.c_str());
//     // std::size_t found = s.rfind("/");
//     // if (found != std::string::npos){
//     //     std::string tmp = s.substr(found + 1);
//     //     cgiFile = new char[tmp.size() + 1];
//     //     std::strcpy(cgiFile, tmp.c_str());
//     // }
//     std::cout << "cgiFile is " << cgiFile << std::endl;
// }

// check if this is valid for both GET and POST
//GUL CHANGED THIS FUNCTION
void Cgi::setCgiFile(std::string s) {
    // finding the position of the '?' character in the url
    std::size_t query_pos = s.find('?');
        if (query_pos != std::string::npos) {
        s = s.substr(0, query_pos);
    }
    cgiFile = new char[s.size() + 1];
    std::strcpy(cgiFile, s.c_str());
}

	void  printSafeString(const SafeString& str) 
	{
		std::ostream_iterator<char> out(std::cout);
		std::copy(str.getData(), str.getData() + str.getLength(), out);
		std::cout << std::endl;
	}
	char*  getSafeString(const SafeString& str)
	{
		size_t strLen = str.getLength() + 1;
		char * charPtr = new char[strLen];
		std::copy(str.getData(), str.getData() + strLen, charPtr);

		return charPtr;
	}

void Cgi::setPostBody(HttpRequest& req){
	std::string s = req.getBody();
	SafeString safeString(s);
	postBody = getSafeString(safeString);
	std::cout << "________SafeString______" << std::endl;
	printSafeString(safeString);
}


void Cgi::setContentLen(HttpRequest& req) {
    contentLen = req.getContentLength();
}

// parse in tmp and copy it to char* env
void Cgi::setCgiEnv(HttpRequest& req, Location& loc, Server& ser){
    std::vector<std::string> tmp;

    tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
    tmp.push_back("SERVER_NAME=" + ser.getHost()); //server hostname
    tmp.push_back("SERVER_SOFTWARE=webserv/1.0");
    tmp.push_back("SERVER_PROTOCOL=HTTP/1.1");
    tmp.push_back("SERVER_PORT=" + std::to_string(ser.getPort())); //server port
    tmp.push_back("REQUEST_METHOD=" + req.getMethod()); //request method
    tmp.push_back("PATH_INFO=" + loc.getRoot() + req.getURI()); // <<< full path to the cgi file
    tmp.push_back("SCRIPT_NAME=/index.py"); //cgi pass
    tmp.push_back("DOCUMENT_ROOT=" + loc.getRoot()); //location getRoot()
    tmp.push_back("QUERY_STRING=" + req.getQueryString()); //getQuery
    // if (req.getMethod() == "POST"){
    //     tmp.push_back("CONTENT_TYPE=" + req.getContentType()); // ex. text/html
    //     tmp.push_back("CONTENT_LENGTH=" + std::to_string(req.getContentLength()));
    // }
    this->env = new char*[tmp.size() + 1];
    int i = 0;
    for (std::vector<std::string>::iterator t = tmp.begin(); t != tmp.end(); ++t){
        this->env[i] = new char[(*t).size() + 1];
        strcpy(this->env[i], (*t).c_str());
        std::cout << env[i] << "ENVVVVVVVVVVVVVVVVVVVVVVVVV" << std::endl;
        ++i;
    }
    this->env[tmp.size()] = NULL;
}

//return http response msg or '\0' in case of internal error
std::vector<char>    Cgi::execCgi(){
    int w_pip[2];
    int r_pip[2];
    pid_t pid;

    std::cout << MAG << "CGI executed"<< RES << std::endl;
    if (pipe(w_pip) < 0 || pipe(r_pip) < 0)
		throw std::runtime_error("pipe failed"); //error
	pid = fork();
	if (pid < 0)
		throw std::runtime_error("fork failed"); //error
	if (pid == 0){
        // close read end and write to pipe 
        //output of cgi script will be written in pipe
        std::cout << MAG << "child process: "<< cgiFile << RES << std::endl;
        // signal(SIGINT, SIG_DFL);
        // signal(SIGTERM, SIG_DFL);
        if (access(cgiFile,F_OK) != 0)
			throw ErrorCodeException(STATUS_NOT_FOUND);
		if (access(cgiFile,X_OK) != 0)
			throw ErrorCodeException(STATUS_FORBIDDEN);
        pass = new char[cgiPass.size() + 1];
        std::strcpy(pass, cgiPass.c_str());
        char *argv[3] = {pass, cgiFile, NULL};
        close(w_pip[0]); 
        if (dup2(w_pip[1], STDOUT_FILENO) < 0)
            throw std::runtime_error("Write write pipe failed"); //error
        close(r_pip[1]);
        if (dup2(r_pip[0], STDIN_FILENO) < 0)
            throw std::runtime_error("Read read pipe failed"); //error
        if (execve(cgiFile, argv, env) < 0){
            throw std::runtime_error("child");
            exit(1);
        }
    }
    int status;
    char buf[BUFFER_SIZE]; // is buffer_size defined in config?
    std::vector<char> body;
    ssize_t bytes = 1;
//close write end and read output from pipe
    close(r_pip[0]);
    if (write(r_pip[1], this->postBody, getContentLen()) < 0)
		throw std::runtime_error ("Write to r_pip failed" );
    close(r_pip[1]);
    close(w_pip[1]);
    if (waitpid(pid, &status, 0) < 0)
        throw std::runtime_error("wait failed"); 
    if (WIFEXITED(status) == false &&(WEXITSTATUS(status)!= 0)) 
        throw std::runtime_error ("wait exit status failed");
    if (dup2(w_pip[0], STDIN_FILENO) < 0)
        throw std::runtime_error ("Write read pipe failed"); // error
    while (bytes > 0){
        std::memset(buf, '\0', BUFFER_SIZE - 1);
        bytes = read(0, buf, BUFFER_SIZE);
        // if (bytes < 0)
            // no read;
        body.insert(body.end(), buf, buf + bytes);
    }
    printf("\n");
    close(w_pip[0]);
    return (body);
}
