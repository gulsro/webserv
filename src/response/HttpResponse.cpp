#include "HttpResponse.hpp"
#include <cctype>

HttpResponse::HttpResponse()
	: version(""), statusCode(0), index(""), resource(""), resourceType(RESOURCE_FILE), content(""), contentType(""), MIMEtype(""), completed(false)
{
	#ifdef STRUCTOR
		std::cout << GREY << "HttpResponse : Default constructor called" << DEFAULT << std::endl; 
	#endif
    this->Request = nullptr;
}

HttpResponse::HttpResponse(const HttpResponse &other)
{
	#ifdef STRUCTOR
		std::cout << GREY << "HttpResponse : Copy constructor called" << DEFAULT << std::endl; 
	#endif
	*this = other;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other)
{
	#ifdef STRUCTOR
		std::cout << GREY << "HttpResponse : Copy assigment operator called" << DEFAULT << std::endl;
	#endif
	if (this != &other)
	{
		this->statusCode = other.statusCode;
		this->body = other.body;
		this->Request = other.Request;
		this->index = other.index;
		this->resource = other.resource;
		this->resourceType = other.resourceType;
		this->content = other.content;
		this->contentType = other.contentType;
		this->MIMEtype = other.MIMEtype;
	}
	return *this;

}

HttpResponse::~HttpResponse()
{
	#ifdef FUNC
		std::cout << GREY << "HttpResponse : Destructor called" << DEFAULT << std::endl; 
	#endif
}

void	HttpResponse::createResponse(enum e_statusCode code)
{
	this->createResponse(code, "");
}

void	HttpResponse::createResponse(enum e_statusCode code, const std::string content)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createResponse" << DEFAULT << std::endl;
	#endif
	this->statusCode = code;

	if (this->statusCode == STATUS_SUCCESS
		|| this->statusCode == STATUS_MOVED
		|| this->statusCode == STATUS_FOUND
		|| this->statusCode == STATUS_NO_CONTENT
		|| this->statusCode == STATUS_CREATED)
	{
		std::ostringstream	ostream;
		ostream << "HTTP/1.1 " << this->statusCode << " " << returnStatusMessage(this->statusCode) << "\r\n";
		if (this->statusCode == STATUS_MOVED || this->statusCode == STATUS_FOUND)
		{
			// Remove leading '.' from resource.
			if (this->resourceType == RESOURCE_FILE)
				ostream << "Location: " << this->resource.substr(1, this->resource.length() - 1) << "\r\n";
			else
				ostream << "Location: " << this->resource.substr(1, this->resource.length() - 1) + "/" << "\r\n";
		}
 		if (!content.empty())
		{
			ostream << "Content-Length: " << content.length() << "\r\n";
			ostream << "Content-Type: text/html\r\n";
		}
		else
			ostream << "Content-Length: 0\r\n";
		ostream << "\r\n";
		if (!content.empty())
			ostream << content;
		this->content = ostream.str(); // a string a copy of ostream
	}
	if (this->statusCode >= 400)
		createErrorResponse(code, this->Request->ReqServer->getErrorPage());
	this->completed = true;
}

void	HttpResponse::createResponse_File(std::string filename)
{
    #ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] createResponse_File" << DEFAULT << std::endl;
	#endif
	std::ifstream	file(filename.c_str());

	if (file.is_open())
	{
		file.seekg(0, std::ios::end); // Move to the end of the file.
		if (file.tellg() > 0) // file is not empty.
		{
			file.seekg(0, std::ios::beg);
			std::ostringstream	ostream;
			std::ostringstream	fileContent;
			fileContent << file.rdbuf();

			ostream << "HTTP/1.1 200 OK\r\n";
			ostream << "Content-Length: " << fileContent.str().length() << "\r\n";
			setMIMEtype(filename);
			ostream << "Content-Type: " << this->getMIMEtype() << "\r\n";
			ostream << "\r\n";
			// print body part
			ostream << fileContent.str();
			file.close();
			this->content = ostream.str();
		}
		else
			createResponse(STATUS_NO_CONTENT);
	}
	else
		createResponse(STATUS_INTERNAL_ERR);
	this->completed = true;
}

void    HttpResponse::setResource()
{
	#ifdef FUNC
	    std::cout << YELLOW << "[FUNCTION] setResource" << DEFAULT << std::endl;
	#endif
	Server	*Server = this->Request->ReqServer;

	if (this->Request->ReqLocation)
	{
		Location	*Location = this->Request->ReqLocation;
		// Check redirection
		if	(!(Location->getRedirect().empty()))
		{
			this->resource = Location->getRedirect();
			createResponse(STATUS_FOUND);
		}
		this->resource = "." + this->Request->getURI();
	}
	else // No selected Location
	{
        std::string uri = this->Request->getURI();
        size_t pos = uri.find(Server->getRoot());
		if (uri == "/")
			this->resource = "." + Server->getRoot() + '/' + Server->getIndex();
		else if (pos != std::string::npos)
        {
            if (uri[pos + Server->getRoot().length()] == '/' || uri[pos + Server->getRoot().length() + 1] == '\0' )
                this->resource = "." + uri;
            else
                this->resource = "." + Server->getRoot() + uri;    
        }
        else
			this->resource = "." + Server->getRoot() + uri;
	}
	#ifdef FUNC
		std::cout << YELLOW  << "RESOURCE : " << this->resource << DEFAULT << std::endl;
	#endif
}
