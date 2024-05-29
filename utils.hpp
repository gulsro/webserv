#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

# define GREEN "\033[0;92m"
# define YELLOW "\e[38;5;208m"
# define RED "\033[1;91m"
# define GREY "\033[0;90m"
# define PURPLE	"\033[38;2;200;160;255m"
# define BLUE "\033[38;5;38m"
# define PINK "\033[95m"
# define DEFAULT "\033[0m"

enum	e_statusCode
{
	STATUS_SUCCESS	= 200,
	STATUS_CREATED = 201,		// The request resulted in a new resource being created on the server. (Location should be contained in the response.)
	STATUS_NO_CONTENT = 204,	// successfully processed the request but there is n content to send back.
	
	STATUS_MOVED = 301,			// The requested resource has been permanently moved to a new location. (New URL should be contained in the Location header.)
	
	STATUS_BAD_REQUEST = 400, 	// Invalid syntax request
	STATUS_UNAUTHORIZED = 401,
	STATUS_FORBIDDEN = 403, 	// No permission to access the requested resource.
	STATUS_NOT_FOUND = 404, 	// The requested resource couldn't be found on the server.(Non-existent page)
	STATUS_NOT_ALLOWED = 405, 	// Not allowed method
	STATUS_REQUSET_TIMEOUT = 408, // Requset timeout
	STATUS_CONFLICT = 409, 		// The request could not be completed due to a conflict with the current state of the resource.
	STATUS_LENGTH_REQUIRED = 411, // Missing Content-Length header
	STATUS_TOO_LARGE = 413, 	// The request entity is too large to handle.
	STATUS_URI_TOO_LONG = 414,
	
	STATUS_INTERNAL_ERR = 500, 	// Unexpected error occurred
	STATUS_NOT_IMPLEMENTED = 501,	// The server doesn't support the fucntionality required
	STATUS_UNAVAILABLE = 503, 	// Service unavailble (overloaded or in maintenance)
	STATUS_GATEWAY_TIMEOUT = 504,	// CGI Timeout
	STATUS_VERSION_NOT_SUPPORTED = 505	// HTTP Version is not supported
};

enum	e_method
{
	GET,
	POST,
	DELETE
};

// Body transmission types
enum	e_transferType
{
	GENERAL, // Content-Length
	CHUNKED // Transfer-Encoding: chunked
};

enum	e_connectionType
{
	CLOSE,
	KEEP_ALIVE
};

struct HttpHeader
{
	std::string key;
	std::string	value;
};

// std::map<std::string, std::string> makeMimeType ()
// {
// 	std::map<std::string, std::string> type_map;

// 	type_map["bin"] = "application/octet-stream";
// 	type_map["bmp"] = "image/bmp";
// }

std::vector<std::string> split(const std::string &str, char delimiter);

std::string trim(const std::string &str);