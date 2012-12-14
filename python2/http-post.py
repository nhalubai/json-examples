#!/usr/bin/env python

"""
This should be the equivalent of

curl foobar3000.com/echo/example.json \
  -X POST \
  -H 'Content-Type: application/json' \
  -d '{
          "answer": 42
      }'
"""

try:
    import json
except ImportError:
    import simplejson as json

try:
    import httplib
except ImportError:
    print("this version of python doesn't have 'httplib'")
    exit(1)

host = 'foobar3000.com'
#port = 80

headers = {
    "Content-Type": "application/json; charset=utf-8"
}
# These are the group of settings adjusted to produce desired Wind Settings
data = {
    "answer": 42
}
body = json.dumps(data)

conn = httplib.HTTPConnection(host)
conn.request('POST', '/echo/example.json', body=body, headers=headers)
#conn.send(json.dumps(data))
res = conn.getresponse()
data = res.read()
print(data)
