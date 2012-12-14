#!/usr/bin/env python3

import json

obj = json.loads('{ "foo": "bar", "baz": [], "grault": {} }')
print(json.dumps(obj, sort_keys=False, indent=4))
