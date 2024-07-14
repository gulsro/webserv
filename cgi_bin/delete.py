#!/usr/bin/env python3

import cgi
import os
import cgitb
cgitb.enable()

status_code = "200 OK"
UPLOAD_DIR = "./html/upload"

request_method = os.environ.get("REQUEST_METHOD", "")

if (request_method == "DELETE"):
	# delete file
	form = cgi.FieldStorage()

from os import walk

f = []
for (dir_path, dir_names, file_names) in walk(UPLOAD_DIR):
	f.extend(file_names)
	break

html_content = ('''
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Delete File</title>
	<style>
	body {
	    font-family: sans-serif;
        margin: 20px;
		margin-bottom: 1em;
	}
	.button {
		padding: 10px;
		background-color: #f0da77;
		color: black;
		border: none;
		cursor: pointer;
		margin-top: 5em;
	}
	.button:hover {
		background-color: #99f077;
	}
	</style>
</head>
<body>
	<a href='/' class="button"> Back to main page </a> <br>
    <label for="file">Delete file:</label>
    <input type="text" id="file" name="file">
    <input type="submit" class="button" value="Delete">
  <br>
  <h2>Files</h2>
  <ul>
''')
for file in f:
	html_content += f"    <li>{file}</li>"

html_content += ('''</body>
	</html>
''')

# Printing response message
print(f"HTTP/1.1 {status_code}", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print("", end="\r\n")
print(html_content)
