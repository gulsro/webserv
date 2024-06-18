
#include "../include/Server.hpp"

Server::Server() : port(0), serverFd(0), root(""), index(""), maxBodySize(1000000)
{
    // std::cout << "Server constructor is called" << std::endl;
}

// Server::Server(int port,
// 				std::string host,
// 				std::vector<std::string> serverNames,
// 				std::string root,
//                 std::string index,
// 				unsigned long maxBodySize
// )
//         : port(port), host(host), serverNames(serverNames),
//         root(root), index(index), maxBodySize(maxBodySize)
// {
//     std::cout << "Server constructor is called" << std::endl;
// }

Server::~Server()
{
    // std::cout << "Server destructor is called" << std::endl;
    for (std::vector<Location *>::iterator i = locationList.begin();
     i != locationList.end();i++){
        delete *i;
        *i = nullptr;
     }
}


Server::Server(const Server& s){
    operator=(s);
}

Server& Server::operator=(const Server s){
    if (this == &s)
        return (*this);
    this->port = s.getPort();
    this->serverFd = s.getServerFd();
    this->host = s.getHost();
    this->root = s.getRoot();
    this->index = s.getIndex();
    this->maxBodySize = s.getMaxBodySize();
    return (*this);
}

//Not necessary because createSock() assigns value for serverFd
// void Server::setServerFd(int fd)
// {
//     this->serverFd = fd;
// }

//--------------Getters-------------------
int Server::getPort() const
{
    return this->port;
}

std::string Server::getHost() const
{
    return this->host;
}

int Server::getServerFd() const
{
    return this->serverFd;
}

std::string Server::getRoot() const
{
    return this->root;
}

struct sockaddr_in* Server::getSocketAddr() const
{
    return const_cast<sockaddr_in*>(&serverAddr);
}

//std::vector<std::string> getServerNames() const;

// std::vector<int> Server::getConnectedClientFds() const
// {
//     return connectedClientFds;
// }

std::ostream& operator<<(std::ostream& out, const Server& server)
{
    out << "port: " << server.getPort();
    out << std::endl;
    out << "serverFd: " << server.getServerFd();
    out << std::endl;
    out << "host: " << server.getHost();
    out << std::endl;
    //out << server.serverNames;
    out << "root: " << server.getRoot();
    out << std::endl;

    return out;
}

void Server::createSocket()
{
    this->serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverFd == -1)
        throw std::runtime_error("Error: socket()");
}

void Server::setSocketAddr()
{
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
}

void Server::setSocketOption()
{
    //This allows me to reuse a socket address
    //even if it's still in the TIME_WAIT state from a previous connection.
    //this is helpful when you quickly
    //restart a server and want to bind to the same port without waiting
    int optval = 1;
    if (setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &optval,
                        sizeof(optval)) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: setsockopt()");
    }
}

void Server::bindSocket()
{
    if (bind(this->serverFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: bind()");
    }
}

void Server::listenSocket()
{
    if (listen(this->serverFd, 10) == -1)
    {
        close(this->serverFd);
        throw std::runtime_error("Error: listen()");
    }
}

void Server::printConnectedClientFds() const
{
    for (auto &clientFd: this->connectedClientFds)
    {
        std::cout << "Connected clientFd: " << clientFd << std::endl;
    }
}

std::string Server::getIndex() const
{
    return this->index;
}

unsigned long Server::getMaxBodySize() const
{
    return this->maxBodySize;
}

std::vector<Location*> Server::getLocationList() const
{
    return this->locationList;
}


//--------------Setters-------------------
//return needs to be an error

void Server::setPort(std::string& cont, int key){
    if (getPort())
        return ;
    while(std::isspace(cont[key]))
        key++;
    size_t colon = cont.find(':');
    if (colon != std::string::npos){
        std::string host = cont.substr(key, colon - key);
        setHost(host);
        this->port = std::stoi(cont.substr(colon + 1, cont.find('\n') - colon + 1));
    }
    else {
        this->port = std::stoi(cont.substr(key, cont.find('\n') - key + 1));
    }
}

void Server::setHost(std::string& cont){
    if (!getHost().empty())
        return ;
    this->host = cont;
}
// void Server::setServerFd(int fd){

// }

void Server::setRoot(std::string& cont, int key){
    if (!getRoot().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->root = cont.substr(key, cont.find('\n') - key);
}

void Server::setIndex(std::string& cont, int key){
    if (cont.find("autoindex") != std::string::npos)
        return ;
    if (!getIndex().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->index = cont.substr(key, cont.find('\n') - key);
}


void Server::setMaxBodySize(std::string& cont, int key){
    if (getMaxBodySize())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->maxBodySize = std::stoi(cont.substr(key, cont.find('\n') - key));
}

//--------------Functions-------------------

void Server::setServerVar(std::stringstream& iss)
{
    std::size_t key;
    std::string line;
    std::string parameter[4] = {"listen", "root", "index", "client_size"};
     while (std::getline(iss, line, '\n')){
        for (int i = 0; i < 4; i++){
            key = line.find(parameter[i]);
            if(key != std::string::npos)
                (this->*func[i])(line, key + parameter[i].size());
        }
    }
    // std::cout << YEL << *this << RES << std::endl;
}

std::size_t Server::skipLocationPath(std::string cont, std::size_t found){
    while (std::isspace(cont[found]))
        found++;
    if (cont[found] != '/' && cont[found] != '*')
        throw std::runtime_error("Location misses a path");
    while (!std::isspace(cont[found]))
        found++;
    return (found);
}

void Server::splitLocation(std::string cont){
    std::size_t found = cont.find("location");
    while (found != std::string::npos){
        std::size_t begin = skipLocationPath(cont, found + 8);
        begin = findScopeBegin(cont, begin);
        std::size_t end = findScopeEnd(cont, begin);
        this->locationCont.push_back(cont.substr(found + 9, end - found - 8));
        this->nbLocation++;
        begin = end + 1;
        found = cont.find("location", begin, 8);
    }
    if (!this->nbLocation)
        throw std::runtime_error("No server was found in config file");
    // std::cout << "# of Location is: " << nbLocation << std:: endl;
    // for (std::string s : locationCont)
    //     std::cout << BLU << s << RES << std::endl;
}

void Server::initLocation(std::string serverCont){
    for (std::string cont : this->locationCont){
        std::stringstream iss(cont);
        Location* l = new Location(this);
        l->setLocationVar(iss);
        l->checkLocationVar(serverCont);
        this->locationList.push_back(l);
    }
}

std::ostream& operator<<(std::ostream& out, const Server& server)
{
    out << "port: " << server.getPort()<< std::endl;
    out << "serverFd: " << server.getServerFd()<< std::endl;
    out << "host: " << server.getHost()<< std::endl;
    //out << server.serverNames;
    out << "root: " << server.getRoot()<< std::endl;
    out << "index: " << server.getIndex()<< std::endl;
    out << "Max body size: " << server.getMaxBodySize()<< std::endl;
    out << std::endl;
    return out;
}
