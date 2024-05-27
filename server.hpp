#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h> // socker operations
//# include <arpa/inet.h>
# include <cstring> // memset()
# include <netdb.h> // getaddrinfo()
# include <unistd.h> // close()
# include <string> // to_string()

// The server will use the Host header from incoming HTTP requests
// to determine which virtual host configuration to apply.

class Server:
{
    private:
        std::string serverName;
        int port;

    public:
        Server();

        
}