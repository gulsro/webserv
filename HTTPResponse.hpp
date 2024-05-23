#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

class HTTPResponse
{
	protected:
		std::string	version;
		int			statusCode;
		std::string	statusMessage;
		std::unordered_map<std::string, std::string> headers;
		std::string	body;

	public:
		HTTPResponse();
		HTTPResponse(const HTTPResponse &response);
		HTTPResponse &operator=(const HTTPResponse &other);
		virtual ~HTTPResponse();
};
