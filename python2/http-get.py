#!/usr/bin/env python

"""
This should be the equivalent of

curl foobar3000.com/echo/example.json \
  -X GET \
"""

try:
    import urllib2 as urllib
except ImportError:
    import urllib

import sys

try:
    fp = urllib.urlopen("http://foobar3000.com/echo/example.json")
except:
    print("couldn't connect:", sys.exc_info()[0])
    exit(1)

data = fp.read()
print(data)
