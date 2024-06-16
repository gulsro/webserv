#include "Webserv.hpp"

// void readRequest(int clientFd)
// {
//     //Response response(clientFd);
// 	//response.handle_request();

//     // int byteRead;

// 	// if (byteRead = read(clientFd, &buf, BUF_SIZE)) < 0)
//     // {
//     //     if (byte_read != write(clientFd, "KOLONYAAAA", byte_read))
//     //         exit(1);
//     // }

//     write(clientFd, "KOLONYAAAA", 11);
//     if (close(clientFd) == -1)
//         exit(1);
// }

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    // Declare a unique_ptr without initializing
    std::unique_ptr<ServerManager> serverManager;    
    
    // Later, assign a dynamically allocated object to it
    // The std::make_unique function creates a new instance of ServerPool on the heap
    // and returns a std::unique_ptr<ServerPool> that owns this instance
    serverManager = std::make_unique<ServerManager>();

    // Use the unique_ptr
    //std::cout << "ServerManager at main.cpp: " << *serverManager << std::endl;

    serverManager->startServerManager(*serverManager); 
    serverManager->startPoll();

    // serverManager automatically cleans up the memory when it goes out of scope

}