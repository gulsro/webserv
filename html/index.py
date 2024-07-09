#!/usr/bin/python3

# print("Content-Type: text/html\r\n\r\n")

# print()

from cgi import FieldStorage

form = FieldStorage()
pageId = form.getvalue("id")

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