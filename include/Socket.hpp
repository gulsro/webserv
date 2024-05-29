#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h> // socker operations
//# include <arpa/inet.h>
# include <cstring> // memset()
# include <netdb.h> // getaddrinfo()
# include <unistd.h> // close()
# include <string> // to_string()

// One socket class to define instance sockets for client and sever

class Socket
{
    public:
        Socket();
        ~Socket();    

};

std::ostream operator<<(std::ostream& out, const Socket& socket);

#endif