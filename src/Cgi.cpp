#include "../includes/Cgi.hpp"

#define BUFFER_SIZE 1024
#define WRITE_SIZE 1024

/*Because you won’t call the CGI directly, use the full path as PATH_INFO.
∗ Just remember that, for chunked request, your server needs to unchunk
it, the CGI will expect EOF as end of the body.
∗ Same things for the output of the CGI. If no content_length is returned
from the CGI, EOF will mark the end of the returned data.
∗ Your program should call the CGI with the file requested as first argument.
∗ The CGI should be run in the correct directory for relative path file access.
∗ Your server should work with one CGI (php-CGI, Python, and so forth)*/


Cgi::Cgi(){}

Cgi::Cgi(HttpRequest& req, Location& loc, Server& ser, ServerManager &sManager)
: pass(NULL), postBody(NULL), contentLen(0), manager(&sManager), pipeRead(-1), pipeWrite(-1), childPid(-1), finishReading(false)
{
    cgiPass = loc.getCgiPass();
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
    for (int i = 0; this->env[i] != NULL; ++i) {
        delete[] this->env[i];
    }
    delete[] this->env;
    //delete[] this->env;
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
////////////////////////////////////////////////////////////////////////////////////////////////////////
void    Cgi::childDup(int ToCgi[2], int FromCgi[2]){
    close(ToCgi[1]);
    if (dup2(ToCgi[0], STDIN_FILENO) < 0)
        throw std::runtime_error("Read read pipe failed");
    close(ToCgi[0]);
    close(FromCgi[0]); 
    if (dup2(FromCgi[1], STDOUT_FILENO) < 0)
        throw std::runtime_error("Write write pipe failed");
    close(FromCgi[1]); 
}

void    Cgi::initParentPipe(int ToCgi[2], int FromCgi[2]){
    #ifdef CGI
		std::cout << PINK << "[ Cgi ] initParentPipe" << DEFAULT << std::endl; 
	#endif
    close(ToCgi[0]);
    close(FromCgi[1]);
    pipeWrite = ToCgi[1];
    pipeRead = FromCgi[0];
     #ifdef CGI
		std::cout << PINK << "pipeWrite is " << pipeWrite << "   pipeRead is " << pipeRead << DEFAULT << std::endl; 
	#endif
}

void    Cgi::writeToCgi(){

    #ifdef CGI
		std::cout << PINK << "[ Cgi ] writeToCgi" << DEFAULT << std::endl; 
	#endif
    #ifdef FUNC
		std::cout << PINK << "[ Cgi ] writeToCgi" << DEFAULT << std::endl; 
	#endif
    if (cgiInput.size() == 0){
        manager->addFdToPollFds(pipeRead, POLLIN);
        manager->rmFdFromPollfd(pipeWrite);        // manager.removeEvent(pipeWrite, POLLOUT);
        close(pipeWrite);
        // pipeWrite = -1;
    }

	size_t writeSize = WRITE_SIZE;

	if (WRITE_SIZE >= this->cgiInput.size()) {
		this->cgiInput.push_back('\0');
		writeSize = this->cgiInput.size();
	}
    ssize_t bytes = write(this->pipeWrite, this->cgiInput.data(), writeSize); //cgiInput stores a whole request incl
    if (bytes < 0)
		throw std::runtime_error ("Writing to CGI has failed" );
    else if (bytes < WRITE_SIZE)
        bytes = write(this->pipeWrite, "\0", 1); //is it needed?
    cgiInput.erase(cgiInput.begin(),  cgiInput.begin() + bytes);
    manager->addFdToPollFds(pipeWrite, POLLOUT);
}

void    Cgi::readFromCgi(){
    #ifdef CGI
		std::cout << PINK << "[ Cgi ] readFromCgi" << DEFAULT << std::endl; 
	#endif
    #ifdef FUNC
		std::cout << PINK << "[ Cgi ] readFromCgi" << DEFAULT << std::endl; 
	#endif
    std::vector<char> buf(BUFFER_SIZE);
    ssize_t bytes = 1;

    // std::memset(buf, '\0', BUFFER_SIZE - 1);
    bytes = read(this->pipeRead, buf.data(), BUFFER_SIZE);
    if (bytes < 0)
        std::runtime_error("Reading from CGI has failed.");
     if (bytes== 0){
        finishReading = true;
        // manager->rmFdFromPollfd(pipeRead);        // manager.removeEvent(pipeWrite, POLLOUT);
        // close(pipeRead);
    }
    cgiOutput.insert(cgiOutput.end(), buf.begin(), buf.begin() + bytes);
    // manager->addFdToPollFds(pipeRead, POLLIN);

    // this->appendReadBytes += bytes;
    // bytes_read += bytes; // do we need to add bytes 
}

void    Cgi::execCGI()
{
    
    #ifdef CGI
		std::cout << PINK << "[ Cgi ] execCGI" << DEFAULT << std::endl; 
	#endif
    #ifdef FUNC
		std::cout << PINK << "[ Cgi ] execCGI" << DEFAULT << std::endl; 
	#endif
    int r_pip[2]; // pipe to cgi
    int w_pip[2]; // pipe from cgi
    pid_t pid;

    pass = new char[cgiPass.size() + 1];
    std::strcpy(pass, cgiPass.c_str());
    char *argv[3] = {pass, cgiFile, NULL};

    //Initialize pipes
    if (pipe(r_pip) < 0)
		throw std::runtime_error("pipe failed");
    if (pipe(w_pip) < 0){
        close (r_pip[0]);
        close (r_pip[1]);
		throw std::runtime_error("pipe failed");
    }
    // Executing cgi script
	pid = fork();
	if (pid < 0){
        close (w_pip[0]);
        close (w_pip[1]);
        close (r_pip[0]);
        close (r_pip[1]);
		throw std::runtime_error("fork failed"); 
    }
    else if (pid == 0){ //child process
        std::cout << MAG << "child process: "<< cgiFile << RES << std::endl;
        childDup(r_pip, w_pip);
        if (access(cgiFile,F_OK) != 0)
			throw ErrorCodeException(STATUS_NOT_FOUND);
		if (access(cgiFile,X_OK) != 0)
			throw ErrorCodeException(STATUS_FORBIDDEN);
        if (execve(cgiFile, argv, env) < 0){
            throw std::runtime_error("child");
            exit(1);
        }
    }
    else {
        //parent process
        initParentPipe(r_pip, w_pip);
        if (cgiInput.size() > 0) // if there is any data to send to CGI
            manager->addFdToPollFds(pipeWrite, POLLOUT); // keep
        else { 
            //if there's nothing to send to CGI, we will trigger reading the output from pipe
            close(pipeWrite);
            std::cout << "all input is sent to cgi pipe -> Now it should execute pipe read is " << pipeRead <<std::endl;
            pipeWrite = -2; //add condition for 
            manager->addFdToPollFds(pipeRead, POLLIN);
        }
    }
}

void    Cgi::putRequestIntoCgiInput(const std::string rawRequest)
{
    #ifdef CGI
		std::cout << PINK << "[ Cgi ] putRequestIntoCgiInput" << DEFAULT << std::endl; 
	#endif
    for (size_t i = 0; i < rawRequest.size(); ++i)
        this->cgiInput.push_back(rawRequest[i]);
}

bool	Cgi::isRunningCgi()
{
	return (childPid != -1 && waitpid(childPid, NULL, WNOHANG) == 0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////


//return http response msg or '\0' in case of internal error
// std::vector<char>    Cgi::execCgi(){
//    //PipeFromCGI
//     int w_pip[2]; // 3. Children write the output to the w_pipe -> 4. parent read and parse it to Response Body
//      //PipeToCGI
//     int r_pip[2]; // 1.  parent write body received from STDIN -> 2. Children read from pipe
//     pid_t pid;

//     std::cout << MAG << "CGI executed"<< RES << std::endl;
//     if (pipe(w_pip) < 0 || pipe(r_pip) < 0)
// 		throw std::runtime_error("pipe failed"); //error
// 	pid = fork();
// 	if (pid < 0)
// 		throw std::runtime_error("fork failed"); //error
// 	if (pid == 0){
//         // close read end and write to pipe 
//         //output of cgi script will be written in pipe
//         std::cout << MAG << "child process: "<< cgiFile << RES << std::endl;
//         // signal(SIGINT, SIG_DFL);
//         // signal(SIGTERM, SIG_DFL);
//         if (access(cgiFile,F_OK) != 0)
// 			throw ErrorCodeException(STATUS_NOT_FOUND);
// 		if (access(cgiFile,X_OK) != 0)
// 			throw ErrorCodeException(STATUS_FORBIDDEN);
//         pass = new char[cgiPass.size() + 1];
//         std::strcpy(pass, cgiPass.c_str());
//         char *argv[3] = {pass, cgiFile, NULL};
//         close(w_pip[0]); 
//         if (dup2(w_pip[1], STDOUT_FILENO) < 0)
//             throw std::runtime_error("Write write pipe failed"); //error
//         close(r_pip[1]);
//         if (dup2(r_pip[0], STDIN_FILENO) < 0)
//             throw std::runtime_error("Read read pipe failed"); //error
//         if (execve(cgiFile, argv, env) < 0){
//             throw std::runtime_error("child");
//             exit(1);
//         }
//     }
//     int status;
//     char buf[BUFFER_SIZE]; // is buffer_size defined in config?
//     std::vector<char> body {};
//     ssize_t bytes = 1;
// //close write end and read output from pipe
    
//     //If we type an input
//     close(r_pip[0]);
//     if (strlen(this->postBody) > 0){
//         manager->addFdToPollFds(r_pip[1], POLLOUT);
//     }
//     else{
//         close(r_pip[1]);
//         manager->addFdToPollFds(w_pip[0], POLLIN);
//     }

//     if (write(r_pip[1], this->postBody, getContentLen()) < 0) //GETCONTENTLEN() WILL BE CHECKED
// 		throw std::runtime_error ("Write to r_pip failed" );
    
    
//     close(r_pip[1]);
//     close(w_pip[1]);
//     if (waitpid(pid, &status, 0) < 0)
//         throw std::runtime_error("wait failed"); 
//     if (WIFEXITED(status) == false &&(WEXITSTATUS(status)!= 0)) 
//         throw std::runtime_error ("wait exit status failed");
//     if (dup2(w_pip[0], STDIN_FILENO) < 0)
//         throw std::runtime_error ("Write read pipe failed"); // error
//     while (bytes > 0){
//         std::memset(buf, '\0', BUFFER_SIZE - 1);
//         bytes = read(0, buf, BUFFER_SIZE);
//         // if (bytes < 0)
//             // no read;
//         body.insert(body.end(), buf, buf + bytes);
//     }
//     printf("\n");
//     close(w_pip[0]);
//     return (body);
// }