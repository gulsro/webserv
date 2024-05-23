#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

class	HTTPRequest
{
	protected:
		std::string	method;
		std::string	uri;
		std::string version;
		std::unordered_map<std::string, std::string> headers;
		std::string	body;

	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &response);
		HTTPRequest &operator=(const HTTPRequest &other);
		virtual ~HTTPRequest();

	bool	parseHttpRequest(const std::string &rawRequest);
	bool	parseRequestLine(const std::string &line);
	bool	parseHeader(const std::string &line);
	bool	parseBody(const std::string &line);

};