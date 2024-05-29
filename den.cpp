#include <string>

class VirtualServer {
public:
    VirtualServer(const std::string &address, int port, const std::string &serverName);
    std::string getAddress() const;
    int getPort() const;
    std::string getServerName() const;
    int getServerFD() const;
    void setServerFD(int fd);
    bool matchesHostAndPort(const std::string &host, int port) const;

private:
    std::string address;
    int port;
    std::string serverName;
    int server_fd;
};

VirtualServer::VirtualServer(const std::string &address, int port, const std::string &serverName)
    : address(address), port(port), serverName(serverName), server_fd(-1) {}

std::string VirtualServer::getAddress() const {
    return address;
}

int VirtualServer::getPort() const {
    return port;
}

std::string VirtualServer::getServerName() const {
    return serverName;
}

int VirtualServer::getServerFD() const {
    return server_fd;
}

void VirtualServer::setServerFD(int fd) {
    server_fd = fd;
}

bool VirtualServer::matchesHostAndPort(const std::string &host, int port) const {
    return this->address == host && this->port == port;
}





#include <iostream>
#include <vector>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>
#include "VirtualServer.hpp"  // Include the VirtualServer header

class Server {
public:
    Server(const std::vector<VirtualServer> &virtualServers);
    void run();

private:
    std::vector<VirtualServer> virtualServers;
    std::vector<pollfd> fds;

    void setup();
    void acceptConnection(int server_fd);
    void handleClient(int client_fd);
    void makeSocketNonBlocking(int fd);
    VirtualServer* getVirtualServer(int fd);
};

Server::Server(const std::vector<VirtualServer> &virtualServers) : virtualServers(virtualServers) {
    setup();
}

void Server::setup() {
    for (auto &vServer : virtualServers) {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, vServer.getAddress().c_str(), &server_addr.sin_addr);
        server_addr.sin_port = htons(vServer.getPort());

        if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 10) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        makeSocketNonBlocking(server_fd);

        pollfd pfd = { server_fd, POLLIN, 0 };
        fds.push_back(pfd);

        vServer.setServerFD(server_fd);
    }
}

void Server::makeSocketNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void Server::run() {
    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                VirtualServer* vServer = getVirtualServer(fds[i].fd);
                if (vServer) {
                    acceptConnection(fds[i].fd);
                } else {
                    handleClient(fds[i].fd);
                }
            }
        }
    }
}

VirtualServer* Server::getVirtualServer(int fd) {
    for (auto &vServer : virtualServers) {
        if (vServer.getServerFD() == fd) {
            return &vServer;
        }
    }
    return nullptr;
}

void Server::acceptConnection(int server_fd) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }

    makeSocketNonBlocking(client_fd);

    pollfd pfd = { client_fd, POLLIN, 0 };
    fds.push_back(pfd);
    std::cout << "New connection accepted." << std::endl;
}

void Server::handleClient(int client_fd) {
    char buffer[1024];
    int bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read <= 0) {
        close(client_fd);
        fds.erase(std::remove_if(fds.begin(), fds.end(), [client_fd](pollfd &pfd) {
            return pfd.fd == client_fd;
        }), fds.end());
        std::cout << "Connection closed." << std::endl;
        return;
    }

    buffer[bytes_read] = '\0';
    std::cout << "Received request: " << buffer << std::endl;

    // Parse the HTTP request to determine the virtual server
    std::string host = parseRequestHost(buffer);
    int port = getRequestPort(client_fd);

    VirtualServer* vServer = nullptr;
    for (auto &vs : virtualServers) {
        if (vs.matchesHostAndPort(host, port)) {
            vServer = &vs;
            break;
        }
    }

    if (vServer) {
        // Delegate the handling to the appropriate VirtualServer
        vServer->handleRequest(client_fd, buffer, bytes_read);
    } else {
        const char *response = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
        write(client_fd, response, strlen(response));
        close(client_fd);
        fds.erase(std::remove_if(fds.begin(), fds.end(), [client_fd](pollfd &pfd) {
            return pfd.fd == client_fd;
        }), fds.end());
    }
}
