#include "Webserv.hpp"

void tempConfigServer(ServerManager& serverManager)
{
    server_ myServer;
    myServer.port = 8085;
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
                myServer.root, myServer.index, myServer.maxBodySize));

    server_ myServer1;
    myServer1.port = 8081;
    myServer1.serverFd = -2;
    myServer1.host = "127.0.0.1";
    myServer1.serverNames = {"example1.com", "www.example1.com"};
    myServer1.root = "/var/www/html1";
    myServer1.index = "index.html";
    myServer1.maxBodySize = 1048576;

//     servers.push_back(Server(myServer1.port,
//                 myServer1.host, myServer1.serverNames,
//                 myServer1.root, myServer1.index, myServer1.maxBodySize, serverManager));

// Server s2(myServer1.port,
//                 myServer1.host, myServer1.serverNames,
//                 myServer1.root, myServer1.index, myServer1.maxBodySize, serverManager);

// serverManager.addServer(*s2);

serverManager.addServer(std::make_unique<Server>(myServer1.port,
                myServer1.host, myServer1.serverNames,
                myServer1.root, myServer1.index, myServer1.maxBodySize));
}