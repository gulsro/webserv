#pragma once

#include <unordered_map>
#include "utils.hpp"
#include <cstdlib> // remove(), std::system()


enum	resourceType
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
		std::string			resource;
		enum resourceType	resourceType;
		std::string			content;
		std::string			contentType;
		std::string			location;

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

		HttpRequest					*getRequest() const { return Request }
		const	 std::string			&getResource() const { return resource + Request->getIndex() }
		const	std::string			&getContent() { return content }

		std::string	getStatusMessage();
		void		checkMethod();
};
