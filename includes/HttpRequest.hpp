#pragma once

#include <unordered_map>
#include "utils.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"

class	HttpResponse;
class	Location;
class	Server;


typedef	struct	s_part
{
	std::string	partContentType;
	std::string	partFilename;
	std::string	data;
	std::vector<std::pair<std::string, std::string>> pairs;
}	t_part;

class	HttpRequest
{
	protected:
		std::string	method;
		std::string	uri;
		std::string version;
		std::unordered_map<std::string, HttpHeader> headers;
		std::string	body;
		int			contentLength;
		std::string	contentType;

		int			requestedPort;
        Server      *ReqServer;
        Location	*ReqLocation;
		
		//multi-part form-data
		std::string	boundaryBegin;
		std::string	boundaryEnd;
		std::map<int, t_part>	parts;	
		
		//cgi environment
		std::vector<std::pair<std::string, std::string>>	queryPairs;

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
		const	std::string	&getContentType() const { return contentType; }
		const	int	       	&getRequestedPort() const { return requestedPort; } 
		const	std::vector<std::pair<std::string, std::string>> &getQueryPairs() const { return queryPairs; }

		//Setters
		void	setContentLength(int len) { this->contentLength = len; }
		void	setRequestedPort();
        void    setReqServer(const std::vector<Server*> serverList);
		void	setReqLocation(const std::vector<Location*> locationList);
		void	setContentType();
		void	setBoundary();
		void	setQueryPairs();
		
		bool	readRequest(int fd);
		bool	parseHttpRequest(const std::string &rawRequest);
		bool	parseRequestLine(const std::string &line);
		bool	parseHeader(const std::string &line);
		const	std::string	getHeaderValue(const std::string &name) const;
		void	checkContentType();
		void	checkRequestValid();
		void	checkLocations();
		void	checkUriValid();
		void	checkAllowedMethods();
		void	handleMultiPartForm();
		void	makeKeyValuePair(int i, const std::string str);
        void    handlePartInfo(const int i, const std::vector<std::string> strs);

		std::vector<std::string> splitByBoundary();
};