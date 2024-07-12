#!/usr/bin/python3

import cgi, cgitb
import os

cgitb.enable()

print ("Content-type:text/html\r\n")
print()

form = cgi.FieldStorage()
# Get filename here
fileitem = form["filename"]

# Test if the file was uploaded
if fileitem.file:
   fn = os.path.basename(fileitem.file)
   open(os.getcwd() + '/cgi-bin/tmp/' + fn, 'wb').write(fileitem.file.read())
   message = 'The file "' + fn + '" was uploaded to ' + os.getcwd() + '/cgi-bin/tmp'
else:
   message = 'Uploading Failed'

# print("<H1> " + message + " </H1>")

print ('''<!DOCTYPE html>
Content-Type: text/html\n
<html>
<body>
   <p>%s</p>
</body>
</html>
''' % (message,))
