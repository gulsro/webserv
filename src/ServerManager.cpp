#include "Webserv.hpp"

extern volatile sig_atomic_t gSignal;

ServerManager::ServerManager(const Config& config)
{
  // Loop and copy raw pointers (not recommended)
  for (Server* server : config.serverList) {
            servers.emplace_back(server);
        }
	isWritingDone = false;
}

ServerManager::~ServerManager()
{
    for (std::vector<Server *>::iterator i = servers.begin(); i != servers.end(); i++){
        delete *i;
        *i = nullptr;
    }
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


void ServerManager::addServer(Server* server)
{
    this->servers.push_back(server);
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


void ServerManager::startServers()
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

        mapServerFd[server->getServerFd()] = server;
    }
}

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
    clientLastActivity[clientFd] = std::chrono::system_clock::now();
    setNonBlocking(clientFd);
    Client* client = new Client(clientFd, READ);

    server.clientList.push_back(client);
    server.connectedClientFds.push_back(clientFd);
    mapClientFd[clientFd] = client;
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
        int num_readyFds = poll(pollfds.data(), pollfds.size(), -1);  // Wait indefinitely
        if (num_readyFds == -1){
            continue ;
        }
        checkTimeouts();
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

            if (revents & POLLIN)
            {
                // if a server received a request. let's accept a client
                if (isFdInMap(fd, mapServerFd)) //fd is one of the server's fd
                {
                    Server* server = mapServerFd[fd];
                    acceptClient(fd, *server); //that client is accepted by
                }
                else //continue reading operations on connected clients
                    handleIncoming(fd);
            }

            // Here check writing operation's klaarheid.
            //client is ready for WHICH operation FLAG needs to be added!
            else if (revents & POLLOUT)
            {
				if (isPipeFd(fd) == false && mapClientFd[fd]->getReadyToFlag() == WRITE)
                {
                    sendResponse(fd);
					isWritingDone = true;
                }
                else
                {
                    Client *currClient;
                    int clientFd = getClientFdOfPipe(fd);
					currClient	= mapClientFd[clientFd];
                    currClient->getCgi()->writeToCgi();
                }
                
            }
            // Handle events for accepted connections (read/write data)
            // You'll need to iterate over other servers and their connections here
			else if (revents & POLLHUP)
			{
				Client *currClient;
				if (isPipeFd(fd) == false)
				{
					currClient = mapClientFd[fd];
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
						rmFdFromPollfd(fd);
                        currClient->setClientFdEvent(pollfds, POLLOUT);
					}
				}
			}
        }
    }
}

void ServerManager::checkTimeouts() {
    auto now = std::chrono::system_clock::now();

    for (auto it = clientLastActivity.begin(); it != clientLastActivity.end();) {
        int fd = it->first;
        auto lastActivity = it->second;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastActivity).count() > TIMEOUT) {
            rmFdFromPollfd(fd);
            mapClientFd.erase(fd);
            it = clientLastActivity.erase(it);
            close(fd);
        } 
        else 
            ++it;
    }
}

Server* ServerManager::getServer(int serverFd) const
{
    auto it = mapServerFd.find(serverFd);
    if (it != mapServerFd.end()){
        return it->second;
    }
    throw std::runtime_error("Server not found");
}

//AT THIS POINT WE DECIDE CGI? IN READREQUEST()?
int ServerManager::handleIncoming(int fd)
{
	#ifdef CGI
		std::cout << YELLOW << "[FUNCTION] handleIncoming" << DEFAULT << std::endl;
	#endif

	Client *currClient;

    //if fd is pipe, -> pass it to cgi read or execution
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
		if (currClient->getResponse() == nullptr)
			currClient->setResponse(new HttpResponse());
		if (isPipeFd(fd) == true && currClient->getCgi()->getFinishReading() == false)
			currClient->getCgi()->readFromCgi();
		else
		{
			if (this->readRequest(currClient) == true) //continue reading operations on connected clients
				currClient->getRequest()->setReqServer(servers);
			{
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
            if (currClient->getCgi() != nullptr)
                currClient->finishCgi();
            currClient->getResponse()->setContent(e.what());
            currClient->getResponse()->setCompleted(true);
            currClient->setReadyToFlag(WRITE);
            currClient->setClientFdEvent(pollfds, POLLOUT);
        }
        else
            std::cout << e.what() << std::endl;
        return 0;
	}
	// else continue reading
    if (currClient->getReadyToFlag() == WRITE)
    {
        if (currClient->getCgi() != NULL )
            return 0;
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
        return ;
    try
    {
		if (isPipeFd(fd) == false && currClient->getResponse() != nullptr && currClient->getResponse()->getCompleted() == false)
		{
        	currClient->getResponse()->setRequest(currClient->getRequest());
        	currClient->getResponse()->checkMethod();
		}
        else if (currClient->getResponse()->getContent().empty())
            throw ErrorCodeException(STATUS_INTERNAL_ERR, currClient->getRequest()->getReqServer().getErrorPage());
    }
    catch(const std::exception& e)
    {
        if (currClient)
        {
            currClient->getResponse()->setContent(e.what());
            currClient->getResponse()->setCompleted(true);
            currClient->setReadyToFlag(WRITE);
            currClient->setClientFdEvent(pollfds, POLLOUT);
        }
        else
            std::cout << e.what() << std::endl;
    }
	#ifdef CGI
		std::cout << GREEN << "-----------RESPONSE---------------" << std::endl;
		std::cout << currClient->getResponse()->getContent() << DEFAULT << std::endl;
	#endif
	ssize_t bytesSent = send(fd, currClient->getResponse()->getContent().c_str(),
                        currClient->getResponse()->getContent().size(), 0);
	if (bytesSent == -1 || static_cast<size_t>(bytesSent) != currClient->getResponse()->getContent().size())
	{
        rmFdFromPollfd(fd);
        delete currClient->getRequest();
        delete mapClientFd[fd];
        mapClientFd[fd] = nullptr;
        std::cerr << "Error: send()" << std::endl;
        return ;
	}
    rmFdFromPollfd(fd);
    close(fd);
}

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
	// Iterates through all clients and find if the clients has a pipe with same fd.
	for (const auto& [clientFd, clientPtr] : mapClientFd)
	{
        if (clientPtr != nullptr && clientPtr->getCgi() != nullptr)
        {
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

bool    ServerManager::isFdInPollfds(int targetFd)
{
    for (const auto& pollfd : pollfds)
    {
        if (pollfd.fd == targetFd)
            return true;
    }
    return false;
}
