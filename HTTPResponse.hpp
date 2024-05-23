#pragma once

#include <iostream>
#include <string>

class HTTPResponse
{
	protected:
		std::string	version;
		int			statusCode;
		std::string	statusMessage;
		std::string	date;
		std::string	server;
		std::string	lastModified;
		std::string	eTag;
		std::string	acceptRange;
		std::string	contentlength;
		bool		connection;
		std::string	contentType;

		std::string	body;

	public:
		HTTPResponse();
		HTTPResponse(const HTTPResponse &response);
		HTTPResponse &operator=(const HTTPResponse &other);
		virtual ~HTTPResponse();
};
