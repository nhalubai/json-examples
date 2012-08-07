#!/usr/bin/env bash
TARGET='http://localhost:3000/incoming'

BACKWARDS='"serial": "0460",'

if [ 0 -eq `expr ${RANDOM} % 2` ]
then
  STARTPAIR='"success": true,'
  ENDPAIR=''
else
  # test if this will work with an old version that's missing an expected key
  BACKWARDS=''
  STARTPAIR=''
  ENDPAIR='", success": true'
fi

RANDKEY=`echo "obase=16; "${RANDOM}"" | bc`
RANDVAL=`echo "obase=16; "${RANDOM}"" | bc`
curl "${TARGET}" \                                                          
    -X POST \
    -H "Content-Type: application/json" \
    -d '{
          '${STARTPAIR}'
          "errors": [],
          '${BACKWARDS}'
          "timestamp": 1336507831211,
          "result": [
              {
                  "id": "575",
                  "timestamp": 1327615150035,
                  "geolocation": {
                      "latitude": 40.324956,
                      "longitude": "-111.672006",
                      "bearing": null,
                      "heading": null,
                      "altitudeAccuracy": null,
                      "speed": null,
                      "altitude": 1458.3,
                      "accuracy": null
                  },
                  "observation": {
                      "elevationAngle": null,
                      "azimuthAngle": 186.869295,
                      "horizontalAngle": -43.130705,
                      "radialVelocity": -13.190949,
                      "range": 384,
                      "verticalAngle": null
                  },
                  "stats": {
                      "rcs": 1.641635
                  }
              }
          ]
          '${ENDPAIR}'
      }'
