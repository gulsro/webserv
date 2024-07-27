# Webserv

*Webserv is writing own non-blocking HTTP server in C++ from scratch. It supports essential HTTP methods (**GET**, **POST**, **DELETE**) and offersthe capaility to excute **CGI scripts.***

---
## Usage

1. **Building**
```
$ make
```
2. **Starting the Server**
```
$ ./webserv configs/default.conf
```
3. **Accessing the Server** : Open a web browser and navigate to `http://localhost:8080` or the corresponding hostanme and port defined in out configuration file. 
4. **Stopping the Server** : Press `Ctrl + C` in the terminal where `webserv` is running.

---
## Configuration File
The configuration file format is inspired by **NGINX**. It allows defining server blocks that house location blocks. Each block can have its own set of attributes for customization. \
 \
**Example Configuration File**
```
server {
	listen			localhost:8080 #host:listening port
	root			/html #root folder of site directory, full or relative path
	index			index.html #default page when requesting a directory, index.html by default
	methods			GET #allowed methods in location, GET only by default
	error_page		400 400.html
	error_page		403 403.html
	error_page		404 404.html
    error_page		405 405.html
	error_page		409 409.html
	error_page		413 413.html
	error_page		414 414.html
    error_page		501 501.html
	max_body_size	7000000

	location /download {
		max	2000
		methods			GET
		return			/html/redirect.html
	}
	location /bin {
		methods		DELETE
	}
	location /upload {
		autoindex		on
		methods		GET POST DELETE
	}
	location *.py {
		root		/cgi_bin
		methods		GET POST DELETE
		cgi_pass	/usr/bin/python3 #location of interpreters installed on the current system
	}
}
```
---
## Requirements
- Webserv must be **non-blcoking** and ues **only 1 `poll()` for all the I/O operations** between the client and the server (including listen).
- `poll()` must check read and write at the same time.
- Reading and writing operation without going through `poll()` is forbidden.
- Checking the value of `errno` is forbidden after a read or a write operation.
- A request to Webserv never hang forever.
- Webserv must be able to listen to multiple ports.
- Set a default file to answer if the request is a directory. (*index index.html in our configuration file*)
- Turn on and off directory listing. (*autoindex  on in our configuration file*)
- Make CGI work with POST and GET methods.
---
### **Contributors**
<table>
   <td align="center"><a href="https://github.com/eunbi0308"><img src="https://avatars.githubusercontent.com/u/80834766?v=4" width="70px;" alt=""/><br /><sub><b>eunbi0308</b></sub></a><br /><a
  <tr>
    <td align="center"><a href="https://github.com/gulsro"><img src="https://avatars.githubusercontent.com/u/115263972?v=4" width="70px;" alt=""/><br /><sub><b>gulsro</b></sub></a><br /><a
  </tr>
    <td align="center"><a href="https://github.com/yuka110"><img src="https://avatars.githubusercontent.com/u/106901790?v=4" width="70px;" alt=""/><br /><sub><b>yuka110</b></sub></a><br /><a
 <table>
