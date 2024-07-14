#!/usr/bin/python3

import cgi, cgitb, urllib
#cgi.parse_qs is deprecated
import os

# header for the response
status_code = "200 OK"

form = cgi.FieldStorage()
# print(form)

# if "id " not in form:
#     pageId = "BANANA"
# else:
query_string = os.environ.get("QUERY_STRING", "")
parameters = urllib.parse.parse_qs(query_string)
pageId = parameters.get("id", [""])[0]
title=pageId

#pageId = form.getvalue("id")


html_content = (f'''<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>My Simple Website</title>
</head>
<body>
  <h1>{title}</h1>
  <p>This is a very basic HTML page.</p>
  <ol>
    <li><a href="index.py?id=HTML">HTML</a></li>
  <li><a href="index.py?id=CSS">CSS</a></li>
  <li><a href="index.py?id=JavaScript">JavaScript</a></li>
  </ol>
</body>
</html>
''')

print(f"HTTP/1.1 {status_code}", end="\r\n")
print("Content-Type: text/html", end="\r\n"), 
print(f"Content-Length: {len(html_content)}", end="\r\n")
print("", end="\r\n")
print(html_content)

#Most often, CGI scripts live in the server’s special cgi-bin directory.
# The HTTP server places all sorts of information about the request
# (such as the client’s hostname, the requested URL, the query string, and lots of other goodies)
#in the script’s shell environment, executes the script, and sends the script’s output back to the client.

#The script’s input is connected to the client too, and sometimes the form data is read this way;
# at other times the form data is passed via the “query string” part of the URL.
# #This module is intended to take care of the different cases and
# provide a simpler interface to the Python script. It also provides
# a number of utilities that help in debugging scripts,
# and the latest addition is support for file uploads from a form (if your browser supports it).