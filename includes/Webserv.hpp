#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <string>
#include <sstream> // std::stringstream
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo()
#include <sys/types.h> //accept()
#include <map>
#include <poll.h> // poll() and struct
#include <fcntl.h>
#include <memory> // std::unique_ptr c++11, std::make_unique c++14
#include <fstream>
#include <istream>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <csignal>
#include <cstring>
#include <functional>
#include <sys/wait.h>

#include "Config.hpp"
#include "Server.hpp"
#include "ServerManager.hpp"
#include "Client.hpp"
#include "Location.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Cgi.hpp"
#include "utils.hpp"

#define BOLD "\033[1m"
#define RES "\033[0m"
#define RED "\033[31m"
#define GRE "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define MAG "\033[35m"
#define CYA "\033[36m"
#define WHI "\033[37m"

//volatile?? prevents compiler optimization??
//The volatile keyword tells the compiler that the value of the variable can change at any time,
//even outside the control of the program (for example, due to hardware or a signal handler).
//This prevents the compiler from optimizing away accesses to the variable,
//ensuring that every read or write to the variable actually occurs in the program's flow
//For instance in a for loop it might change anytime.
volatile sig_atomic_t gSignal = 0;

void handle_signal(int signal) {
    (void)signal;
	gSignal = 1;
}


void setup_signal_handler() 
{
    //process performs any necessary cleanup operations before exiting
    signal(SIGINT, handle_signal);

    //the process has the freedom to choose how it handles the signal. It can perform cleanup operations, bette roption
    signal(SIGTERM, handle_signal); 
}

void readRequest(int clientFd);

#endif
