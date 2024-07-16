#!/usr/bin/env python3

import cgi
import os
import cgitb
import urllib
cgitb.enable()

status_code = "200 OK"
UPLOAD_DIR = "./html/upload"

request_method = os.environ.get("REQUEST_METHOD", "")

if (request_method == "DELETE"):
	query_string = os.environ.get("QUERY_STRING", "")
	parameters = urllib.parse.parse_qs(query_string)
	fileToDelete = parameters.get("fileName", [""])[0]
	filePathToDelete = UPLOAD_DIR + "/" + fileToDelete
	if (os.path.exists(filePathToDelete)):
		os.remove(filePathToDelete);
	else:
		status_code = "400 BAD REQUEST"

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
	.delete_button{
		margin : 5px;
	}

	</style>
</head>
''')
html_content += ('''
<body>
	<a href='/' class="button"> Back to main page </a> <br>
  <br>
  <script>
    function deleteFile(fileName) {
		fetch('/delete.py?fileName=' + fileName, { method: 'DELETE' });
		location.reload();
	}
  </script>
  <h2>Files</h2>
  <ul>
''')
for file in f:
	html_content += f'''    <li>{file}
	<button
		onclick="deleteFile('{file}')">
		Delete
	</button></li>'''

html_content += ('''</body>
	</html>
''')

# Printing response message
print(f"HTTP/1.1 {status_code}", end="\r\n")
print("Content-Type: text/html", end="\r\n")
print("", end="\r\n")
print(html_content)
