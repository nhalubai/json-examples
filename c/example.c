// standard stuff
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// curl
#include <curl/curl.h>
#include <curl/easy.h>

// json parser
#include <json-c/json.h>

// resource to get/set
#define RESOURCE "/geolocation.json"
#define TRACKS "/tracks.json"

// should be big enough for most things
// more flexibility may need to be added for a real application
// An average tracks packet is about 1.5k (4 tracks), and settings resources
// are generally < .5k.
#define URL_SIZE 256
#define BUF_SIZE 10*1024

long bytesWritten = 0;
long bytesRead = 0;

static int writeFn(void* buf, size_t len, size_t size, void* userdata) {
    size_t sLen = len * size;

    // if this is zero, then it's done
    // we don't do any special processing on the end of the stream
    if (sLen > 0) {
        // >= to account for terminating null
        if (bytesWritten + sLen >= BUF_SIZE) {
            fprintf(stderr, "Buffer size exceeded.\n  Buffer length: %d\n  Current length: %d\n  Bytes to write: %d\n", BUF_SIZE, bytesWritten, sLen);
            return 0;
        }

        memcpy(&((char*)userdata)[bytesWritten], buf, sLen);
        bytesWritten += sLen;
    }

    return sLen;
}

int getSettings(char* url, char* data) {
    int res = -1;
    CURL* pCurl = curl_easy_init();

    if (!pCurl) {
        return 0;
    }

    bytesWritten = 0;
    memset(data, 0, BUF_SIZE);

    // setup curl
    curl_easy_setopt(pCurl, CURLOPT_URL, url);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFn);
    // we don't care about progress
    curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(pCurl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, data);

    // set a 1 second timeout
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 3);

    // synchronous, but we don't really care
    res = curl_easy_perform(pCurl);

    // cleanup after ourselves
    curl_easy_cleanup(pCurl);

    return res;
}

int getSettingsGzip(char* url, char* data) {
    int res = -1;
    CURL* pCurl = curl_easy_init();

    int res1;

    if (!pCurl) {
        return 0;
    }

    bytesWritten = 0;

    // setup curl
    curl_easy_setopt(pCurl, CURLOPT_URL, url);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFn);
    // we don't care about progress
    curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(pCurl, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, data);

    // add the gzip header
    curl_easy_setopt(pCurl, CURLOPT_ACCEPT_ENCODING, "gzip;q=1.0");

    // set a 1 second timeout
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 3);

    // synchronous, but we don't really care
    res = curl_easy_perform(pCurl);

    // cleanup after ourselves
    curl_easy_cleanup(pCurl);

    return res;
}

static size_t read_callback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t tLen;
    char* str;
    if (!userdata) {
        return 0;
    }

    str = (char*)userdata;
    tLen = strlen(&str[bytesRead]);
    if (tLen > size * nmemb) {
        tLen = size * nmemb;
    }

    if (tLen > 0) {
        // assign the string as the data to be sent
        memcpy(ptr, &str[bytesRead], tLen);
        bytesRead += tLen;
    }

    return tLen;
}

int postSettings(char* url, char* data) {
    int res = -1;
    char tmp[BUF_SIZE];
    CURL* pCurl = curl_easy_init();

    // we need to set headers later
    struct curl_slist* headers = NULL;

    if (!pCurl) {
        return 0;
    }

    bytesWritten = 0;
    bytesRead = 0;

    // we'll use data to store the result
    memset(tmp, 0, BUF_SIZE);

    // add the application/json content-type
    // so the server knows how to interpret our HTTP POST body
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // setup curl
    curl_easy_setopt(pCurl, CURLOPT_URL, url);
    curl_easy_setopt(pCurl, CURLOPT_POST, 1);
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, strlen(data));
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(pCurl, CURLOPT_READDATA, data);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFn);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, tmp);
    // we don't care about progress
    curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(pCurl, CURLOPT_FAILONERROR, 1);

    // set a 1 second timeout
    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 3);

    // synchronous, but we don't really care
    res = curl_easy_perform(pCurl);

    // cleanup after ourselves
    curl_easy_cleanup(pCurl);
    curl_slist_free_all(headers);

    // copy the response to data
    memcpy(data, tmp, BUF_SIZE);
    return res;
}

void handleSettings(char* data) {
    struct json_object* settingsJson;
    struct json_object* results;
    struct json_object* altitudeObj;
    struct json_object* sendObj;
    double alt;

    // parse the string into json
    settingsJson = json_tokener_parse(data);

    // get the results object
    // this is common among all outputs
    results = json_object_object_get(settingsJson, "result");

    // get the altitude as a double
    // we'll change this conditionally and give it back
    altitudeObj = json_object_object_get(results, "altitude");

    // parse it as a double
    alt = json_object_get_double(altitudeObj);

    printf("Current altitude: %f\n", alt);

    // do some work with this
    // for this example, we'll just do something useless
    if (alt > 2000) {
        // something to signify that this worked
        alt = 747;
    } else {
        // increment to show that this worked
        alt += 1000;
    }

    printf("New altitude: %f\n\n", alt);

    sendObj = json_object_new_object();
    altitudeObj = json_object_new_double(alt);
    json_object_object_add(sendObj, "altitude", altitudeObj);

    // return the updated info
    memset(data, 0, BUF_SIZE);
    strcpy(data, json_object_to_json_string(sendObj));
}

void setTrackUrl (char* url, char* host) {
    int urlLength = 0;

    // copy the host first
    strcpy(url, host);

    urlLength = strlen(url);
    // remove the trailing /
    if (url[urlLength - 1] == '/') {
        url[urlLength - 1] = '\0';
        urlLength--;
    }

    // add the resource to the end of the URL
    strcpy(&url[urlLength], TRACKS);
}

void processTrack (char* data) {
    struct json_object* settingsJson;
    struct json_object* results;
    struct json_object* currentTrack;
    struct json_object* id;

    // geolocation structures
    struct json_object* geolocation;
    struct json_object* latitude;
    struct json_object* longitude;
    struct json_object* altitude;
    struct json_object* accuracy;
    struct json_object* altitudeAccuracy;
    struct json_object* bearing;
    struct json_object* heading;
    struct json_object* speed;

    // observation structures
    struct json_object* observation;
    struct json_object* range;
    struct json_object* radialVelocity;
    struct json_object* horizontalAngle;
    struct json_object* azimuthAngle;
    struct json_object* verticalAngle;
    struct json_object* altitudeAngle;

    // stats structures
    struct json_object* stats;
    struct json_object* rcs;

    struct json_object* timestamp;

    int length = 0;
    int i = 0;

    // parse the data into JSON struct
    settingsJson = json_tokener_parse(data);

    /*
        select the "result" of the settings -- note there is only one "result"
        array even for multiple tracks
    */
    results = json_object_object_get(settingsJson, "result");

    // get the length (the number of tracks in the array)
    length = json_object_array_length(results);

    // iterate over the array of tracks
    for (i = 0; i < length; i++) {
        currentTrack = json_object_array_get_idx(results, i);

        id = json_object_object_get(currentTrack, "id");

        // get geolocation data
        geolocation = json_object_object_get(currentTrack, "geolocation");
        latitude = json_object_object_get(geolocation, "latitude");
        longitude = json_object_object_get(geolocation, "longitude");
        altitude = json_object_object_get(geolocation, "altitude");
        accuracy = json_object_object_get(geolocation, "accuracy");
        altitudeAccuracy = json_object_object_get(geolocation, "altitudeAccuracy");
        bearing = json_object_object_get(geolocation, "bearing");
        heading = json_object_object_get(geolocation, "heading");
        speed = json_object_object_get(geolocation, "speed");

        // get observation data
        observation = json_object_object_get(currentTrack, "observation");
        range = json_object_object_get(observation, "range");
        radialVelocity = json_object_object_get(observation, "radialVelocity");
        horizontalAngle = json_object_object_get(observation, "horizontalAngle");
        azimuthAngle = json_object_object_get(observation, "azimuthAngle");
        verticalAngle = json_object_object_get(observation, "verticalAngle");
        altitudeAngle = json_object_object_get(observation, "altitudeAngle");

        // get stats data
        stats = json_object_object_get(currentTrack, "stats");
        rcs = json_object_object_get(stats, "rcs");

        timestamp = json_object_object_get(currentTrack, "timestamp");

        printf("\nTrack\n");
        printf("    id: %d\n", json_object_get_int(id));
        printf("    geolocation:\n");
        printf("        latitude: %f\n", json_object_get_double(latitude));
        printf("        longitude: %f\n", json_object_get_double(longitude));
        printf("        altitude: %f\n", json_object_get_double(altitude));
        printf("        accuracy: %f\n", json_object_get_double(accuracy));
        printf("        altitudeAccuracy: %f\n", json_object_get_double(altitudeAccuracy));
        printf("        bearing: %f\n", json_object_get_double(bearing));
        printf("        heading: %f\n", json_object_get_double(heading));
        printf("        speed: %f\n", json_object_get_double(speed));
        printf("    observation:\n");
        printf("        range: %f\n", json_object_get_double(range));
        printf("        radialVelocity: %f\n", json_object_get_double(radialVelocity));
        printf("        horizontalAngle: %f\n", json_object_get_double(horizontalAngle));
        printf("        azimuthAngle: %f\n", json_object_get_double(azimuthAngle));
        printf("        verticalAngle: %f\n", json_object_get_double(verticalAngle));
        printf("        altitudeAngle: %f\n", json_object_get_double(altitudeAngle));
        printf("    stats:\n");
        printf("        rcs: %f\n", json_object_get_double(rcs));
        printf("    timestamp: %ld\n", json_object_get_int64(timestamp));
    }
}

int main(int argc, char** argv) {
    // variable to hold return values for error checking
    int res;
    // keep track of where in the buffer to add fragments of URLs
    int iLen;

    char url[URL_SIZE];
    char trackUrl[URL_SIZE];
    char data[BUF_SIZE];
    char trackData[BUF_SIZE];

    // print usage if the input doesn't match what is expected
    if (argc != 2) {
        printf("Usage: example <url>\n  url: address of a spotter (e.g. 169.254.254.254)\n");
        return -1;
    }

    // clear our memory
    memset(url, 0, sizeof(url));
    memset(trackUrl, 0, sizeof(trackUrl));
    memset(data, 0, BUF_SIZE*sizeof(char));
    memset(trackData, 0, BUF_SIZE*sizeof(char));

    // the url is the second arg
    strcpy(url, argv[1]);
    iLen = strlen(url);
    // remove the trailing /
    if (url[iLen - 1] == '/') {
        url[iLen - 1] = '\0';
        iLen--;
    }

    // add the resource to the end of the URL
    strcpy(&url[iLen], RESOURCE);

    // get the current settings
    res = getSettings(url, data);
    if (res != 0) {
        // error occurred
        fprintf(stderr, "CURL error: %d\n", res);
        return res;
    }

    // output the starting settings
    printf("Original Settings:\n%s\n\n", data);

    // make sense of the data received and make changes
    handleSettings(data);

    // settings can only be set by posting to /resource/settings
    iLen = strlen(url);
    strcpy(&url[iLen], "/settings");

    // set our new and improved settings
    res = postSettings(url, data);
    if (res != 0) {
        fprintf(stderr, "CURL error (from postSettings): %d\n", res);
        return res;
    }

    printf("Result from POST:\n%s\n\n", data);

    memset(data, 0, BUF_SIZE);

    // get the current settings, but gzipped
    res = getSettingsGzip(url, data);
    if (res != 0) {
        fprintf(stderr, "CURL error (from getSettingsGzip): %d\n", res);
        return res;
    }

    // output our new settings
    printf("New Settings (gzipped response):\n%s\n", data);

    // set the url to get track information
    setTrackUrl(trackUrl, argv[1]);

    // get the current track information
    res = getSettings(trackUrl, trackData);
    if (res != 0) {
        fprintf(stderr, "CURL error (from getSettings): %d\n", res);
        return res;
    }

    // iterate over all tracks
    processTrack(trackData);

    return res;
}
