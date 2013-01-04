#!/usr/bin/env python

"""
This should be the equivalent of

curl foobar3000.com/echo/example.json \
  -X GET \
"""

from urllib.request import urlopen, Request
import sys
import json

url = Request("http://foobar3000.com/echo/example.json")
try:
    fp = urlopen(url)
except:
    print("couldn't connect:", sys.exc_info()[0])
    exit(1)

data = fp.read().decode("utf-8")
print(data)
