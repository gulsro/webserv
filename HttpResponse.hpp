#pragma once

#include <unordered_map>
#include "utils.hpp"


class HttpRequest;

class HttpResponse
{
	protected:
		std::string	version;
		int			statusCode;
		std::string	statusMessage;
		std::unordered_map<std::string, HttpHeader> headers;
		std::string	body;

	public:
		HttpResponse();
		HttpResponse(const HttpResponse &response);
		HttpResponse &operator=(const HttpResponse &other);
		virtual ~HttpResponse();

		//Getter
		const	std::string	&getVersion() const { return version; }
		int		getStatusCode()	const { return statusCode; }
		const	std::string	&getStatusMessage() const { return statusMessage; }
		const	std::unordered_map<std::string, HttpHeader> &getHeaders() const { return headers; }
		const	std::string	&getBody() const { return body; }

		void	parseHttpResponse(const std::string &raw);
};
