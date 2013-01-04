#!/usr/bin/env python

import json

obj = json.loads('{ "foo": "bar", "baz": [], "grault": {} }')
print(json.dumps(obj, sort_keys=False, indent=4))
