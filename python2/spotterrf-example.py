import json
import urllib
import httplib
import sys


def exit_unless_compatible(url):
    compatible = True

    try:
        fp = urllib.urlopen(url + 'version.json')
    except:
        print("couldn't connect:", sys.exc_info()[0])
        exit(1)

    try:
        obj = json.loads(fp.read())
    except:
        print("couldn't parse data", sys.exc_info()[0])
        exit(1)

    ver = obj.get('result')
    if ver:
        major = ver.get('major')
        if major and 3 == int(major):
            minor = ver.get('minor')
            if minor and int(minor) >= 0:
                compatible = True
                print("Compatible, doing all the things")

    if not compatible:
        print("Not Compatible, Doing Nothing")
        exit()


def count_tracks(url):
    fp = urllib.urlopen(url + 'tracks.json')
    data = fp.read()
    print(data)
    obj = json.loads(data)
    print(str(len(obj.get('result'))))


def change_settings(host, port):
    headers = {
        "Content-Type": "application/json; utf-8"
    }
    # These are the group of settings adjusted to produce desired Wind Settings
    detectionSettings = {
        "sensitivity": 7
    }
    trackSettings = {
        "minDetection": 6, "rangeThreshold": 4, "activationTime": 2
    }

    conn = httplib.HTTPConnection(host, port)
    conn.request('POST', '/detections.json/settings', headers=headers)
    conn.send(json.dumps(detectionSettings))
    res = conn.getresponse()
    try:
        print(res.read())
    except:
        print("couldn't get response:", sys.exc_info()[0])

    conn = httplib.HTTPConnection(host, port)
    conn.request('POST', '/tracks.json/settings', headers=headers)
    conn.send(json.dumps(trackSettings))
    res = conn.getresponse()
    try:
        print(res.read())
    except:
        print("couldn't get response:", sys.exc_info()[0])

if '__main__' == __name__:
    url = "http://remote.spotterrf.com:7773/"
    host = 'remote.spotterrf.com'
    port = 7773
    exit_unless_compatible(url)
    count_tracks(url)
    change_settings(host, port)
