#!/usr/bin/env python

from urllib.request import urlopen, Request
from http.client import HTTPConnection
import json
import sys


def exit_unless_compatible(url):
    compatible = True

    url = Request(url + 'version.json')
    try:
        fp = urlopen(url)
    except:
        print("couldn't connect:", sys.exc_info()[0])
        exit(1)

    try:
        text = fp.read().decode('utf-8')
        obj = json.loads(text)
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
    fp = urlopen(url + 'tracks.json')
    text = fp.read().decode('utf-8')
    print(text)
    obj = json.loads(text)
    print(str(len(obj.get('result'))))


def change_settings(host, port):
    headers = {
        "Content-Type": "application/json; charset=utf-8"
    }
    # These are the group of settings adjusted to produce desired Wind Settings
    detectionSettings = {
        "sensitivity": 7
    }
    trackSettings = {
        "minDetection": 6, "rangeThreshold": 4, "activationTime": 2
    }

    conn = HTTPConnection(host, port)
    body = json.dumps(detectionSettings).encode('utf-8')
    conn.request(
        'POST', '/detections.json/settings', body=body, headers=headers
    )
    #conn.send(body)
    res = conn.getresponse()
    try:
        text = res.read().decode('utf-8')
        print(text)
    except:
        print("couldn't get response:", sys.exc_info()[0])

    conn = HTTPConnection(host, port)
    body = json.dumps(trackSettings).encode('utf-8')
    conn.request(
        'POST', '/tracks.json/settings', body=body, headers=headers
    )
    #conn.send(json.dumps(trackSettings))
    res = conn.getresponse()
    try:
        text = res.read().decode('utf-8')
        print(text)
    except:
        print("couldn't get response:", sys.exc_info()[0])

if '__main__' == __name__:
    url = "http://remote.spotterrf.com:7773/"
    host = 'remote.spotterrf.com'
    port = 7773
    exit_unless_compatible(url)
    count_tracks(url)
    change_settings(host, port)
