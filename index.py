#!/bin/bash
print("Content-Type: text/html")
printf()

# importing cgi module
import cgi
form = cgi-FieldStorage()
# get value of id (querystring)
pageId = form["id"].value
# print(pageId)

print('''<!DOCTYPE html>
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
'''.format(title=pageId))