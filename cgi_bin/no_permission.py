#!/usr/bin/python3

import cgi
import sys

# Set content type and blank line (using standard output methods)
sys.stdout.write("Content-Type: text/html\n\n")

# Get form data using cgi module (doesn't use write)
form = cgi.FieldStorage()
name = form.getvalue("name")
message = form.getvalue("message")

# Build HTML content string
html_content = "<html><body><h1>Hello, {}!</h1><p>Your message: {}</p></body></html>".format(name, message)

