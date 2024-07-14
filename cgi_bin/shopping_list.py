#!/usr/bin/python3

import cgi, cgitb
import os

# to store shopping list items
SHOPPING_LIST_FILE = "./html/shopping_list.txt"

status_code = "200 OK"

# read the shopping list from the file
def read_shopping_list():
    try:
        with open(SHOPPING_LIST_FILE, "r") as file:
            return file.read().splitlines()
    except FileNotFoundError:
        return []

# add an item to the shopping list
def add_item_to_shopping_list(item):
    with open(SHOPPING_LIST_FILE, "a") as file:
        file.write(item + "\n")

request_method = os.environ.get("REQUEST_METHOD", "GET")

#print(f"Debug: Request method - {request_method}")


# init an empty list for shopping items
shopping_list = read_shopping_list()
new_item = ""

if request_method == "POST":
    form = cgi.FieldStorage()
    #print(f"Debug: Form data received - {form}")

    new_item = form.getvalue("item")
    #print(f"Debug: New item raw value - {new_item}")

    #print(f"New item: {new_item}")
    if new_item:
        add_item_to_shopping_list(new_item)
        shopping_list.append(new_item)

html_content = ('''<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Shopping List</title>
</head>
<body>
  <h1>Shopping list from your Mama</h1>
  <form action="shopping_list.py" method="post">
    <label for="item">Add Item:</label>
    <input type="text" id="item" name="item">
    <input type="submit" value="Add">
  </form>
  <h2>Items</h2>
  <ul>
''')

for item in shopping_list:
    html_content += f"    <li>{item}</li>"

html_content += '''  </ul>
</body>
</html>
'''

# Printing response message
print(f"HTTP/1.1 {status_code}", end="\r\n")
print("Content-Type: text/html", end="\r\n"), 
print(f"Content-Lenght: {len(html_content)}", end="\r\n")
print("", end="\r\n")
print(html_content)