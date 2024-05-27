#include "../include/Config.hpp"

Config::Config(){

}

Config::~Config(){

}

/*

Config part:
1. accept only ac = 1 or ac = 2 -> else error
2. if ac = 1, config = default.conf, else av[1] will be config
3. check the validity of config file + accesibility
4. read file -> if empty throw error
5. erase comments and whitespaces in the beginning
6. create another string vector for each server blocks + coount nbServer

Server part:

*/