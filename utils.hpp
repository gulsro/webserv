#pragma once

#include <iostream>
#include <string>
#include <map>

enum	status_code
{
	STATUS_SUCCESS = 200,
	STATUS_CREATED = 201,
	STATUS_NO_CONTENT = 204,
	STATUS_MOVED = 301,
	STATUS_BAD_REQUEST = 400,
	STATUS_FORBIDDEN = 403,
	STATUS_NOT_FOUND = 404,
	STATUS_NOT_ALLOWED = 405,
	STATUS_CONFLICT = 409,
	STATUS_LENGTH_REQUIRED = 411,
	STATUS_TOO_LARGE = 413,
	STATUS_URI_TOO_LONG = 414,
	STATUS_INTERNAL_ERR = 500,
	STATUS_NOT_IMPLEMENTED = 501
};

enum	method
{
	GET,
	POST,
	DELETE
};

enum	uri_type
{
	DIR,
	FILE,
	FILE_TO_CREATE,
	CGI
};

// Body transmission types
enum	transfer_type
{
	GENERAL, // Content-Length
	CHUNKED // Transfer-Encoding: chunked
};

enum	connection_type
{
	CLOSE,
	KEEP_ALIVE
};

struct HttpHeader
{
	std::string key;
	std::string	value;
};

std::map<std::string, std::string> makeMimeType ()
{
	std::map<std::string, std::string> type_map;

	type_map["bin"] = "application/octet-stream";
	type_map["bmp"] = "image/bmp";
}

std::string trim(const std::string &str);