#include "Webserv.hpp"

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
    std::cout << "Value: " << *serverManager << std::endl;




    // serverManager automatically cleans up the memory when it goes out of scope

}