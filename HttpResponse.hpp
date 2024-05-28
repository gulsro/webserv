#pragma once

#include <unordered_map>
#include "utils.hpp"
#include <cstdlib> // remove(), std::system()


enum	e_resourceType
{
	FILE,
	DIR
};

class HttpRequest;

class HttpResponse
{
	protected:
		std::string		version;
		int				statusCode;
		std::string		statusMessage;
		std::string		body;
		std::unordered_map<std::string, HttpHeader> headers;

		HttpRequest			*Request;
		std::string			index;
		std::string			resource;
		enum e_resourceType	resourceType;
		std::string			content;
		std::string			contentType;
		std::string			location;
		std::string			MIMEtype;

		bool				completed; // check if response is already sent 

	public:
		HttpResponse();
		HttpResponse(const HttpResponse &response);
		HttpResponse &operator=(const HttpResponse &other);
		virtual ~HttpResponse();

		//Getter
		const	std::string	&getVersion() const { return version; }
		const	int			&getStatusCode()	const { return statusCode; }
		const	std::string	&getStatusMessage() const { return statusMessage; }
		const	std::string	&getBody() const { return body; }
		const	std::unordered_map<std::string, HttpHeader> &getHeaders() const { return headers; }

		HttpRequest			*getRequest() const { return Request; }
		const	std::string	&getIndex() const { return index; };
		const	std::string	&getResource() const { return resource + this->getIndex(); }
		const	std::string	&getContent() const { return content; }
		std::string			getMIMEtype() const;

		std::string	getStatusMessage();
		void		checkURI();
		void		checkMethod();
		void		methodGet();
		void		methodPost();
		void		methodDelete();

		void		deleteFile();
		void		deleteDir();
		void		createResponse(enum e_statusCode code);
		void		createResponse_File(std::string filename);
};
