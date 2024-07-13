#!/usr/bin/env python3

import cgi
import os
import cgitb

cgitb.enable()

UPLOAD_DIR = "./html/"

print("Content-Type: text/html\r\n\r\n")

if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

def handle_post_request():
    form = cgi.FieldStorage()
    fileitem = form.getvalue("file")
    if fileitem:
        fn = os.path.basename(fileitem)
        file_path = os.path.join(UPLOAD_DIR, fn)

        # write the file to the server
        with open(file_path, 'wb') as f:
            f.write(fileitem.file.read())

        return f"File '{fn}' uploaded successfully!"
    else:
        return f"No file was uploaded. {form}"


request_method = os.environ.get("REQUEST_METHOD", "GET")
upload_message = ""

if request_method == "POST":
    upload_message = handle_post_request()

print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Upload File</title>
</head>
<body>
  <h1>File Upload</h1>
  <form enctype="multipart/form-data" action="moneybird.py" method="post">
    <label for="file">Upload File:</label>
    <input type="file" id="file" name="file">
    <br><br>
    <input type="submit" value="Upload">
  </form>
  <h2>{upload_message}</h2>
</body>
</html>
""")
