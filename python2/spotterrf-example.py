import json
import urllib
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
    print(obj)


if '__main__' == __name__:
    url = "http://remote.spotterrf.com:7773/"
    exit_unless_compatible(url)
    count_tracks(url)
