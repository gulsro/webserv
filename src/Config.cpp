#include "Webserv.hpp"

void tempConfigServer(std::vector<Server>& servers, ServerManager& serverManager)
{
    (void)servers;
    server_ myServer;
    myServer.port = 8080;
    myServer.serverFd = -1;
    myServer.host = "127.0.0.1";
    myServer.serverNames = {"example.com", "www.example.com"};
    myServer.root = "/var/www/html";
    myServer.index = "index.html";
    myServer.maxBodySize = 1048576; // 1 MB
    //declare a server each iteration in servers
    // for (auto &server: servers)
    // {
    //     Server(myServer.port,
    //             myServer.host, myServer.serverNames,
    //             myServer.root, myServer.index, myServer.maxBodySize);
    //     //Server(8080, "0.0.0.0", ""webserv/3.0"", "html", "index.html", nullptr, 700000);
    // }

    // servers.push_back(Server(myServer.port,
    //             myServer.host, myServer.serverNames,
    //             myServer.root, myServer.index, myServer.maxBodySize, serverManager));
    serverManager.addServer(std::make_unique<Server>(myServer.port,
                myServer.host, myServer.serverNames,
                myServer.root, myServer.index, myServer.maxBodySize, serverManager));


    server_ myServer1;
    myServer.port = 8081;
    myServer.serverFd = -2;
    myServer.host = "127.0.0.1";
    myServer.serverNames = {"example1.com", "www.example1.com"};
    myServer.root = "/var/www/html1";
    myServer.index = "index.html";
    myServer.maxBodySize = 1048576;

//     servers.push_back(Server(myServer1.port,
//                 myServer1.host, myServer1.serverNames,
//                 myServer1.root, myServer1.index, myServer1.maxBodySize, serverManager));

serverManager.addServer(std::make_unique<Server>(myServer1.port,
                myServer1.host, myServer1.serverNames,
                myServer1.root, myServer1.index, myServer1.maxBodySize, serverManager));
}