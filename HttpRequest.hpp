#pragma once

#include <unordered_map>
#include "utils.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"

class	HttpResponse;
class	Location;

typedef	struct	s_post
{
	std::string	boundaryBegin;
	std::string	boundaryEnd;
	std::string	filename;
} t_post;

class	HttpRequest
{
	protected:
		std::string	method;
		std::string	uri;
		std::string version;
		std::unordered_map<std::string, HttpHeader> headers;
		std::string	body;
		int			contentLength;

        Location	*Location;
		std::string	location;
		s_post		postStruct;
	
		std::vector<std::pair<std::string, std::string>>	postContent;

	public:
		HttpRequest();
		HttpRequest(const HttpRequest &response);
		HttpRequest &operator=(const HttpRequest &other);
		virtual ~HttpRequest();

		friend class HttpResponse;

		//Getters
		const	std::string &getMethod() const { return method; }
		const	std::string &getURI() const { return uri; }
		const	std::string	&getVersion() const { return version; }
		const	std::unordered_map<std::string, HttpHeader> &getHeaders() const { return headers; }
		const	std::string	&getBody() const { return body; }

        const	std::string &getLocation() const { return location; }

		//Setters
		void	setContentLength(int len) { this->contentLength = len; }
		void	setFilename(std::string	name) { this->filename = name; }

		bool	readRequest(int fd);
		bool	parseHttpRequest(const std::string &rawRequest);
		bool	parseRequestLine(const std::string &line);
		bool	parseHeader(const std::string &line);
		const	std::string	getHeaderValue(const std::string &name) const;
		void    handlePostContents();
		bool	hasBoundary();
		void	findFilename();
		// void	createResponseHelper(enum e_statusCode code);
};