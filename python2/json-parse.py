try:
    import json
except ImportError:
    import simplejson as json

obj = json.loads('{ "foo": "bar", "baz": [], "grault": {} }')
print(json.dumps(obj, sort_keys=False, indent=4))
