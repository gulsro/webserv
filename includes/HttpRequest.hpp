#pragma once

#include <unordered_map>
#include "utils.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Cgi.hpp"


class	HttpResponse;
class	Location;
class	Server;
class	Cgi;

/**
 * A struct for "multipart/data-form" content type.
 * For python cgi script, it's not needed.
 */ 
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
		std::string	rawRequest; // store an appended string

		std::string	method;
		std::string	uri;
		std::string version;
		std::unordered_map<std::string, HttpHeader> headers; // in key and value format
		std::string	body;
		int			contentLength;
		std::string	contentType;

        std::string requestedHost;
		int			requestedPort;
        Server      *ReqServer; // selected server block based on host of a request
        Location	*ReqLocation; // selected location block based on URI of a request
		
		//multi-part form-data
		std::string	boundaryBegin;
		std::string	boundaryEnd;
		std::map<int, t_part>	parts;
		
		//cgi environment
		std::string	queryString;
		std::vector<std::pair<std::string, std::string>>	queryPairs;

		//chunked request
		bool	isChunked;

		bool		isCgi;

	public:
		HttpRequest();
		HttpRequest(const HttpRequest &response);
		HttpRequest &operator=(const HttpRequest &other);
		virtual ~HttpRequest();

		friend class HttpResponse;

		// Getters
		const	std::string	&getRawRequest() const { return rawRequest; }
		const	std::string &getMethod() const { return method; }
		const	std::string &getURI() const { return uri; }
		const	std::string	&getVersion() const { return version; }
		const	std::unordered_map<std::string, HttpHeader> &getHeaders() const { return headers; }
		const	std::string	&getBody() const { return body; }
		const	std::string	&getContentType() const { return contentType; }
		const	int			&getContentLength() const { return contentLength; }
        const   std::string &getRequestedHost() const { return requestedHost; }
		const	int	       	&getRequestedPort() const { return requestedPort; }
		const	std::string	&getQueryString() const { return queryString; }
		const	std::vector<std::pair<std::string, std::string>> &getQueryPairs() const { return queryPairs; }
		const	bool		&getIsChunked() const { return isChunked; }
		const	bool		&getIsCgi() const { return isCgi; }
        const	std::string	getHeaderValue(const std::string &name) const;
		Server		&getReqServer() const { return *ReqServer; }
		Location	&getReqLocation() const { return *ReqLocation; }

		// Setters
		void	setRawRequest(std::string buffer) { this->rawRequest = buffer; }
		void	setContentLength(int len) { this->contentLength = len; }
		void	setRequestedHostAndPort();
        void    setReqServer(const std::vector<Server*> serverList);
		void	setReqLocation(Location *location) { ReqLocation = location; }
		void	selectReqLocation(const std::vector<Location*> locationList);
		void	setContentType();
		void	setBoundary();
		void	setQueryString(std::string str);
		void	setQueryPairs();
		void	setIsChunked(bool value) { this->isChunked = value; }
		void	setIsCgi(bool value) { this->isCgi = value; }
		
        // Parsing HTTP request fucntions
		bool	isReadingRequestFinished(std::string rawRequest);
		bool	parseHttpRequest(const std::string &rawRequest);
		bool	parseRequestLine(const std::string &line);
		bool	parseHeader(const std::string &line);

		// Checkers
		void	checkRequestSize();
		void	checkContentType();
		void	checkRequestValid();
		void	checkLocations();
		void	checkUriValidation();
		void	checkAllowedMethods(std::string requestedMethod);

        // Special content types
		void	handleMultiPartForm();
		void	handleChunkedBody(const size_t bodyStart, const std::string rawRequest);
		void	makeKeyValuePair(int i, const std::string str);
        void    handlePartInfo(const int i, const std::string partInfo);

		std::vector<std::string> splitByBoundary();
};
// debugging
void	printParsedRequest(HttpRequest *Request);