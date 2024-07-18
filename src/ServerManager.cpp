#include "Webserv.hpp"

extern volatile sig_atomic_t gSignal;

ServerManager::ServerManager(const Config& config) {
  // Loop and copy raw pointers (not recommended)
  for (Server* server : config.serverList) {
            servers.emplace_back(server);
        }
	isWritingDone = false;
        //config->serverList.clear(); // Clear temporary list after moving servers
}

// ServerManager::ServerManager()
// {
//     std::cout << "ServerManager constructor is called" << std::endl;
// }

ServerManager::~ServerManager()
{
    std::cout << "ServerManager destructor is called" << std::endl;
    // for (auto server: this->servers)
    //     delete server;
}

void ServerManager::printServers() const
{
    std::cout << "*****************************" << std::endl;
    for (const auto &server: servers)
        std::cout << *server << std::endl;
    std::cout << "*****************************" << std::endl;
}

void ServerManager::printPollFds() const
{
    std::cout << "*****************************" << std::endl;
    for (const auto &pollfd: pollfds)
        std::cout << "FD in POLL: " << pollfd.fd << std::endl;
    std::cout << "*****************************" << std::endl;
}

const std::vector<Server*>& ServerManager::getServers() const
{
    return this->servers;
}

// std::unique_ptr<Server>	ServerManager::getServer(std::string host) const
// {
//     auto &servers = this->getServers();

//     for (auto &server: this->servers)
//     {
//         if (std::string::compare(host, server->getHost()) == 0)
//             //return *this;
//     }
// }

//When you call this->servers.push_back(server);
//you are trying to copy the std::unique_ptr.
//However, copying std::unique_ptr is not allowed because
//it would lead to multiple std::unique_ptr instances
//trying to manage the same resource, violating the unique ownership guarantee.
//Using this->servers.push_back(std::move(server));
//explicitly moves the std::unique_ptr into the vector.
//This transfer of ownership ensures that the vector now
//owns the resource, and the original std::unique_ptr becomes empty.
void ServerManager::addServer(Server* server)
{
    //this->servers.push_back(server);
    this->servers.push_back(server);
}

void ServerManager::startServerManager(ServerManager &serverManager)
{
    auto &servers = serverManager.getServers();
    
    //tempConfigServer(serverManager);
    startSockets();
    for (const auto &server: servers)
    {
        std::cout << "SERVER---- " << *server << std::endl;
        //std::cout << "serverFd: " << server->getServerFd() << std::endl;
    }
}

//The only correct way to set one of the file status flags is:
// first to fetch the current flags with F_GETFL,
//logi- cally OR in the new flag, and then set the flags w F_SETFL.

// set the file descriptor to non-blocking mode
int ServerManager::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void ServerManager::startSockets()
{
    //make a loop here, start socket for each server in servers;
    //assign pollfd for each, then push it to pollfds array
    auto &servers = this->getServers();
    for (auto &server: servers)
    {
        server->createSocket();
        setNonBlocking(server->getServerFd());
        server->setSocketAddr();
        server->setSocketOption();
        server->bindSocket();
        server->listenSocket();
        addFdToPollFds(server->getServerFd(), POLLIN); //monitor incoming connections
       
        //get() returns a pointer to the managed object (server);   
        // mapServerFd.emplace(server->getServerFd(), server);
        mapServerFd[server->getServerFd()] = server;
        //mapServerFd.emplace(server->getServerFd(), server);

    }
    std::cout << "SIZE OF MAP IS: " << mapServerFd.size() << std::endl;
    std::cout << "SIZE OF POLLFD VECTOR IS: " << pollfds.size() << std::endl;
    printServers();
}

// struct pollfd {
//     int fd; /* descriptor to check */
//     short events; /* events of interest on fd */
//     short revents; /* events that occurred on fd */
// };
void ServerManager::addFdToPollFds(int fd, int events)
{
    struct pollfd PollFd;

    PollFd.fd = fd;
    PollFd.events = events;
    this->pollfds.push_back(PollFd);
}

// Establish waiting client connections if poll returns an event on listenerFd
// Adds that clientFd to pollFds vector.
void ServerManager::acceptClient(int serverFd, Server& server)
{
    struct sockaddr_in cliAddr;

    std::memset(&cliAddr, 0, sizeof(cliAddr));
    unsigned int cliLen = sizeof(cliAddr);
    int clientFd = accept(serverFd, (struct sockaddr *)&cliAddr, &cliLen);
    if (clientFd == -1)
    {
        throw std::runtime_error("Error: accept()");
    }
    //std::cout << "MAAAAAA" << server << std::endl;
    setNonBlocking(clientFd);
    Client* client = new Client(clientFd, READ);
    //(void)client;
    //std::cout << "MAAAAAA" << server.getPort() << std::endl;
    server.clientList.push_back(client);
    server.connectedClientFds.push_back(clientFd);
    mapClientFd[clientFd] = client;
    //std::cout << "CLIENT FD " << client->getClientFd() << std::endl;
    //server.printConnectedClientFds();
	//mapClientFd[clientFd] = client;
    //addFdToPollFds(clientFd, (POLLIN | POLLOUT));
    addFdToPollFds(clientFd, (POLLIN));
}


//Serverfd && POLLIN indicates incoming CONNECTION,
//Clientfd && POLLIN indicates incoming data from CONNECTED CLIENT.
//(the client has data ready to be read by the server)
void ServerManager::startPoll()
{
    //The poll() function expects its first argument to be
    //a pointer to an array of pollfd structures. In my case, fds is a vector,
    // not a raw array. However, the data() method provides a way
    //to access the raw memory where the vector elements (the pollfd structures)
    //are stored, essentially treating it like an array.
    while (!gSignal)
    {
        this->printPollFds();
        int num_readyFds = poll(pollfds.data(), pollfds.size(), -1);  // Wait indefinitely
        if (num_readyFds == -1)
        {
            // throw std::runtime_error("Error: setsockopt()");
            // break;
            //'continue' is from the book, not sure.
            continue ;
        }
        //std::cout << pollfds.size() << std::endl;
        
        //counter is to see how many time poll loop turns.
        //int counter = 0;
        // Process ready file descriptors
        for (size_t i = 0; i < pollfds.size(); ++i)
        {
            int fd = pollfds[i].fd;
            int revents = pollfds[i].revents;
            
            //The bitwise AND operation allows you to check if a specific event
            //bit (POLLIN) is present in the revents field.
            //In networking, bits are crucial for representing data packets efficiently and reliably.
            
            //If a client attempts to connect to one of your listener
            //server sockets, this will trigger the POLLIN event on that socket's file descriptor.
            //When the loop iterates through the pollfd vector and
            //encounters a listener socket, the revents & POLLIN condition
            //will evaluate to true because the POLLIN bit is set in the
            //revents flags.

            //std::cout << "THE FISH IS " << fd << std::endl;
            
            //counter = counter+ 1;
            //std::cout << "counter = " << counter << std::endl;
            if (revents & POLLIN)
            {
                // if a server received a request. let's accept a client
                if (isFdInMap(fd, mapServerFd)) //fd is one of the server's fd
                {
                    Server* server = mapServerFd[fd];
                    acceptClient(fd, *server); //that client is accepted by
                    std::cout << "client is accepted" << std::endl;
                                    // *mapServerFd[fd] server
                    //continue ;
                }
                else //continue reading operations on connected clients
                {
                    std::cout << "REQUESTTTTTT" << std::endl;
                     //readRequest(fd);
                    handleIncoming(fd);
                }
            }

            // Here check writing operation's klaarheid.
            //client is ready for WHICH operation FLAG needs to be added!
            else if (revents & POLLOUT)
            {
                std::cout << "THE POLLOUT IS " << fd << std::endl;
                std::cout << "RESPONSEEEEE" << std::endl;
				if (mapClientFd[fd]->getReadyToFlag() == WRITE)
                {
                    sendResponse(fd);
					isWritingDone = true;
                    //pollfds[i].fd = -1;
                }
                
            }
            // Handle events for accepted connections (read/write data)
            // You'll need to iterate over other servers and their connections here
            // ...
			else if (revents & POLLHUP)
			{
				std::cout << "\n\nPOLLHUP \n";
				Client *currClient;
				if (isPipeFd(fd) == false)
				{
					currClient = mapClientFd[fd];
					rmFdFromPollfd(fd);
					// delete currClient->getRequest();
					// delete currClient->getResponse();
					delete mapClientFd[fd];
					mapClientFd[fd] = nullptr;
					close(fd);
				}
				else
				{
					int clientFd = getClientFdOfPipe(fd);
					currClient	= mapClientFd[clientFd];
					if (!(revents & POLLIN))
					{
						currClient->finishCgi();
						this->addFdToPollFds(clientFd, POLLOUT);
						continue;
					}
				}
			}
        }
    }
}

// if (_pollFds[i].revents & POLLIN)
//                 {
//                     if (_pollFds[i].fd == _serverSocket)
//                         acceptConnection();
//                     else
//                         handleClientData(i);
//                 }
//                 else if (_pollFds[i].revents & POLLOUT)
//                     sendClientData(i);

Server* ServerManager::getServer(int serverFd) const
{
	std::cout << GREEN << "serverFd" << DEFAULT << std::endl;
    auto it = mapServerFd.find(serverFd);
    if (it != mapServerFd.end())
    {
        return it->second;
    }
    throw std::runtime_error("Server not found");
}

//AT THIS POINT WE DECIDE CGI? IN READREQUEST()?
int ServerManager::handleIncoming(int fd)
{
	#ifdef FUNC
		std::cout << YELLOW << "[FUNCTION] handleIncoming" << DEFAULT << std::endl;
	#endif
    //readRequest(fd);
	// Server *currServer = this->getServer(fd);

	Client *currClient;

    /* if fd is pipe, -> pass it to cgi read or execution*/
	// check if the fd belongs to a Client fd eventhoug it's pipe.
	if (isPipeFd(fd) ==  true)
	{
		int ClientFd = getClientFdOfPipe(fd);
        if (ClientFd == -1)
        {
            close(fd);
            throw std::runtime_error("pipe doesn't exist in any client");
        }
		currClient = mapClientFd[ClientFd];
	}
	else
		currClient = mapClientFd[fd];
    try
    {
		currClient->setResponse(new HttpResponse);

		// requests from cgi will be handled internally.
		if (isPipeFd(fd) == true)
			currClient->getCgi()->readFromCgi();
		else
		{
			if (this->readRequest(currClient) == true) //continue reading operations on connected clients
			{    //Request.readRequest(fd); fd will be client's

				currClient->getRequest()->setReqServer(servers);
				currClient->getRequest()->checkLocations(); // cgi is detected
				currClient->getRequest()->checkRequestValid();
				// Initial excution after receiving the first cgi request.
				if (currClient->getRequest()->getIsCgi() == true)
					currClient->handleCgiRequest(this); // execute cgi
            }
        }
	}
	catch (const std::exception& e)
	{
        if (currClient)
        {
            currClient->getResponse()->setContent(e.what());
            currClient->getResponse()->setCompleted(true);
        }
        return 0;
	}
	// else continue reading
    
    if (currClient->getReadyToFlag() == WRITE)
    {
        if (currClient->getCgi() != NULL && currClient->getCgi()->isRunningCgi() == false)
            return 0;
        // if (currClient->getCgi() != NULL && currClient->getCgi()->getFinishReading() == true){
        //     std::string tmp(currClient->getCgi()->getCgiOutput().begin(), currClient->getCgi()->getCgiOutput().end()); 
        //     currClient->getResponse()->setContent(tmp); 
        // }
        std::vector<struct pollfd>& pollfds = getPollfds();
        currClient->setClientFdEvent(pollfds, POLLOUT);
    }
    return 0;
}

void	ServerManager::sendResponse(int fd)
{
    //same logic as above, decide cgi or not via fd. it can be clientfd of pipefd
    Client *currClient;

	// check if the fd belongs to a Client fd eventhoug it's pipe.
	if (isPipeFd(fd) ==  true)
	{
		int ClientFd = getClientFdOfPipe(fd);
        if (ClientFd == -1)
        {
            close(fd);
            throw std::runtime_error("pipe doesn't exist in any client");
        }
		currClient = mapClientFd[ClientFd];
	}
	else
		currClient = mapClientFd[fd];

    if (currClient->getRequest() == NULL)
    {
        return ;
    }
    //from client getResponse
	//Checking client is still connected??

	//checking is clientFd is still connected
	//also check "if reading request is done" therefor we need a flag ?
	//if (fd ....)
    try
    {

		if (isPipeFd(fd) == false)
		{
        	currClient->getResponse()->setRequest(currClient->getRequest());
        	// currClient->getResponse()->runCgi(&this);
        	currClient->getResponse()->checkMethod();
		}
        else
            currClient->getCgi()->writeToCgi();
    }
    catch(const std::exception& e)
    {
        currClient->getResponse()->setContent(e.what());
		currClient->getResponse()->setCompleted(true);
    }
    std::cout << GREEN << "-----------RESPONSE---------------" << std::endl;
    std::cout << currClient->getResponse()->getContent() << DEFAULT << std::endl;

	//What's that enum status code???
    std::cout << BLUE << "send FD : " << fd << DEFAULT << std::endl;
	ssize_t bytesSent = send(fd, currClient->getResponse()->getContent().c_str(),
                        currClient->getResponse()->getContent().size(), 0);
	if (bytesSent == -1 || static_cast<size_t>(bytesSent) != currClient->getResponse()->getContent().size())
	{
        rmFdFromPollfd(fd);
        delete currClient->getRequest();
        delete mapClientFd[fd];
        mapClientFd[fd] = nullptr;
        throw std::runtime_error("Error: send()");
	}
	if (isWritingDone == true)
	{
    rmFdFromPollfd(fd);
    delete currClient->getRequest();
    delete currClient->getResponse();
    delete mapClientFd[fd];
    mapClientFd[fd] = nullptr;
    close(fd);
	}
    
	//delete the request, it s done
}

//const std::vector<struct pollfd>& ServerManager::getPollfds() const
std::vector<struct pollfd>& ServerManager::getPollfds()
{
    return this->pollfds;
}


bool ServerManager::isFdInMap(int fd, std::map<int, Server*>& mapServerFd)
{
  // Find any element in the map where the key (file descriptor) matches the provided 'fd'
  auto it = std::find_if(mapServerFd.begin(), mapServerFd.end(),
                         [fd](const auto& pair) { return pair.first == fd; });

  // Return true if a matching element is found, false otherwise
  //it != mapServerFd.end();
  //std::cout << "BOOL = " << a << std::endl;
  return it != mapServerFd.end();
}

bool isFdConnected(int fd, std::vector<int>& connectedClientFds)
{
    if (std::find(connectedClientFds.begin(), connectedClientFds.end(), fd) != connectedClientFds.end())
        return true;
    else
        return false;
}

void	ServerManager::rmFdFromPollfd(int fd)
{
	for (std::vector<struct pollfd>::iterator it = pollfds.begin();
		it != pollfds.end(); it++)
	{
		if (it -> fd == fd)
		{
			pollfds.erase(it);
			break ;
		}
	}
	//close(fd);
}

std::ostream& operator<<(std::ostream& out, const ServerManager& serverManager)
{
    (void)serverManager;
    return out;
}

bool	ServerManager::isPipeFd(int fd)
{
     #ifdef CGI
		std::cout << PINK << "[ Cgi ] isPipeFd" << DEFAULT << std::endl; 
	#endif
    std::cout << YELLOW << "Give Fd: " << fd << std::endl;
	// Iterates throught all clients and find if the clients has a pipe with same fd.
	for (const auto& [clientFd, clientPtr] : mapClientFd)
	{
        if (clientPtr != nullptr && clientPtr->getCgi() != nullptr)
        {
            std::cout << YELLOW << "Read PIPE : " <<  clientPtr->getCgi()->getPipeRead() << std::endl;
            std::cout << YELLOW <<  "Write PIPE : " <<clientPtr->getCgi()->getPipeWrite() << DEFAULT << std::endl;
            if (clientPtr->getCgi()->getPipeRead() == fd || clientPtr->getCgi()->getPipeWrite() == fd)
                return true;
        }
	}
	return false;
}

int		ServerManager::getClientFdOfPipe(int pipeFd)
{
        #ifdef CGI
		std::cout << PINK << "[ Cgi ] getClientFdOfPipe" << DEFAULT << std::endl; 
	#endif
	for (const auto& [clientFd, clientPtr] : mapClientFd)
	{
        if (clientPtr != nullptr && clientPtr->getCgi() != nullptr)
        {
		    if (clientPtr->getCgi()->getPipeRead() == pipeFd || clientPtr->getCgi()->getPipeWrite() == pipeFd)
			    return clientFd;
        
        }
	}
    return -1;
}