#pragma once

 
#include <unordered_map>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::istringstream			stream(str);

	while (std::getline(stream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

class	HttpRequest
{
	protected:
		std::string	method;
		std::string	uri;
		std::string version;
		std::unordered_map<std::string, HttpHeader> headers;
		std::string	body;

	public:
		HttpRequest();
		HttpRequest(const HttpRequest &response);
		HttpRequest &operator=(const HttpRequest &other);
		virtual ~HttpRequest();

		//Getters
		const	std::string &getMethod() const { return method; }
		const	std::string &getURI() const { return uri; }
		const	std::string	&getVersion() const { return version; }
		const	std::unordered_map<std::string, HttpHeader> &getHeaders() const { return headers; }
		const	std::string	&getBody() const { return body; }

	bool	readRequest(int fd);
	bool	parseHttpRequest(const std::string &rawRequest);
	bool	parseRequestLine(const std::string &line);
	bool	parseHeader(const std::string &line);
};