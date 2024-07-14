#!/usr/bin/env python3

import cgi
import os
import cgitb
cgitb.enable()  

# created files will be added into ./html/upload directory
UPLOAD_DIR = "./html/upload"
message = ""
upload_message = ""
file_created = False
status_code = "200 OK"

if not os.path.exists(UPLOAD_DIR):
	os.makedirs(UPLOAD_DIR)

request_method = os.environ.get("REQUEST_METHOD", "")

if request_method == "POST":
	form = cgi.FieldStorage()
	fileitem = form["file"]

	if fileitem.filename:
		file_name = os.path.basename(fileitem.filename)
		file_path = os.path.join(UPLOAD_DIR, file_name)

		if os.path.exists(file_path):
			upload_message = f"{file_name} with same name alreay present on server."
			status_code = "409 Conflict"
		else:
			upload_message = "test"
			with open(file_path, 'wb') as f:
				f.write(fileitem.file.read())
			upload_message = "File" + f" {file_name} uploaded successfully!"
			status_code = "201 Created"
			file_created = True
	else:
		upload_message = "No file was uploaded."
		status_code = "400 Bad Request"

html_content = ("""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Upload File</title>
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
""")

html_content += (f"""
<body>
	<a href='/' class="button"> Back to main page </a> <br>
  <h1>File Upload</h1>
  <form enctype="multipart/form-data" action="upload.py" method="post">
    <label for="file">Upload File:</label>
    <input type="file" id="file" name="file">
    <br><br>
    <input type="submit" value="Upload">
  </form>
  <h2>{upload_message}</h2>
</body>
</html>
""")

# Printing response message
print(f"HTTP/1.1 {status_code}", end="\r\n")
print("Content-Type: text/html", end="\r\n"), 
print(f"Content-Length: {len(html_content)}", end="\r\n")
if (file_created == True):
	print (f"Location: {file_path}", end="\r\n")
print("", end="\r\n")
print(html_content)
