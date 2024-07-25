#include "Webserv.hpp"

Location::Location() {}

Location::Location(Server* s) : server(s), path(""), root(""), maxBodySize(0), autoindex(false), index(""), redirect(""), cgiPass("")
{
    methods["GET"] = 0;
    methods["POST"] = 0;
    methods["DELETE"] = 0;

}

Location::~Location(){
    #ifdef STRUCTOR
		std::cout << GREY << "Location : Destructor called" << DEFAULT << std::endl; 
	#endif
}

Location::Location(const Location& a){
    operator=(a);
}

Location& Location::operator=(const Location a){
    if (this == &a)
        return (*this);
    this->path = a.path;
    this->root = a.root;
    this->autoindex = a.autoindex;
    this->redirect = a.redirect;
    this->methods = a.methods;
    this->cgiPass = a.cgiPass;
    return (*this);
}

//--------------Setters-------------------

//path always starts with slash and no end slash
void Location::setPath(std::string& cont, int key){
    if (!getPath().empty())
        return ;
    while(std::isspace(cont[key]))
        key--;
    if (cont[0] != '/'){
        cont = "/" + cont;
        key++;
    }
	std::string tmp = cont.substr(0, key); 
    this->path = trim(tmp, ' ');
}

void Location::setRoot(std::string& cont, int key){
    if (!getRoot().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->root = cont.substr(key, cont.find('\n') - key);
}

void Location::setMaxBodySize(std::string& cont, int key){
    if (getMaxBodySize())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->maxBodySize = std::stoll(cont.substr(key, cont.find('\n') - key));
}

void Location::setAutoindex(std::string& cont, int key){
    (void)key;
    bool serverAutoIndex = this->server->getAutoindex();
    
    if (serverAutoIndex == true)
        this->autoindex = true;
    else if (serverAutoIndex == false && cont.find("on") != std::string::npos)
        this->autoindex = true;
    else
        this->autoindex = false;
}

void Location::setIndex(std::string& cont, int key){
    if (!getIndex().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->index = cont.substr(key, cont.find('\n') - key);
}

void Location::setRedirect(std::string& cont, int key){
    if (!getRedirect().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->redirect = cont.substr(key, cont.find('\n') - key);
}

void Location::setMethods(std::string& cont, int key){
    (void)key;
    key = 0;
    if (cont.find("GET") != std::string::npos)
        methods["GET"] = 1;
    if (cont.find("POST") != std::string::npos)
        methods["POST"] = 1;
    if (cont.find("DELETE") != std::string::npos)
        methods["DELETE"] = 1;
}

void Location::setCgiPass(std::string& cont, int key){
    if (!getCgiPass().empty())
        return ;
    while(std::isspace(cont[key]))
        key++;
    this->cgiPass = cont.substr(key, cont.find('\n') - key);
}

//--------------Functions-------------------

void Location::setLocationVar(std::stringstream& iss){

    std::size_t key;
    std::string line;
    std::string parameter[8] = {"{", "root", "max_body_size", "autoindex", "index", "return", "methods", "cgi_pass"};
    while (std::getline(iss, line, '\n')){
        for (int i = 0; i < 8; i++){
            key = line.find(parameter[i]);
            if(key != std::string::npos){
                if (i == 0)
                    key -= 1;
                (this->*func[i])(line, key + parameter[i].size());
            }
        }
    }
}

void Location::checkLocationVar(){
    if (path.empty())
        throw std::runtime_error("No path is available for the location");
    if (!maxBodySize)
    {
        if (!(*server).getMaxBodySize())
            this->maxBodySize = 1000000;
        else
            this->maxBodySize = (*server).getMaxBodySize();
    }
    if (this->server->getAutoindex() == true && autoindex == false)
        autoindex = true;
}

std::ostream& operator<<(std::ostream& out, const Location& location)
{
    out << "path: " << location.getPath()<< std::endl;
    out << "root: " << location.getRoot()<< std::endl;
    out << "index: " << location.getIndex()<< std::endl;
    out << "autoindex: " << location.getAutoindex()<< std::endl;
    out << "redirect: " << location.getRedirect()<< std::endl;
    out << "methods: ";
    if (location.getMethods()["GET"] == 1)
        out << "GET ";
    if(location.getMethods()["POST"] == 1)
        out << "POST ";
    if (location.getMethods()["DELETE"] == 1)
        out << "DELETE";
    out << std::endl << "cgi_pass: " << location.getCgiPass() << std::endl;
    out << std::endl;
    return out;
}
