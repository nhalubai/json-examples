/*jshint strict:true devel:true browser:true jquery:true es5:true onevar:true laxcomma:true laxbreak:true eqeqeq:true immed:true latedef:true undef:true unused:true*/
(function () {
  "use strict";

  var $ = window.jQuery
    , url = "http://remote.spotterrf.com:7773/"
    ;

  function exitUnlessCompatible() {
    $.get(url + 'version.json', function (data) {
      var ver = data.result
        , major = ver.major
        , minor = ver.minor
        ;

      if (3 === Number(major) && Number(minor) >= 0) {
        console.log("Compatible, doing all the things");
        // TODO jQuery deferred pipe
        countTracks();
      } else {
        console.error("Not Compatible, Doing Nothing");
      }

      //console.log(data);
      $('#js-viewport').text(JSON.stringify(data, null, '  '));
    }, 'json');
  }

  function countTracks() {
    $.get(url + 'tracks.json', function (data) {
      console.log(data.result.length);
      changeSettings();
    });
  }

  function changeSettings() {
    var headers = {
            "Content-Type": "application/json; charset=utf-8"
        }
        // These are the group of settings adjusted to produce desired Wind Settings
      , detectionSettings = {
            "sensitivity": 7
        }
      , trackSettings = {
            "minDetection": 6, "rangeThreshold": 4, "activationTime": 2
        }
      ;

    // use when to join multiple requests
    $.when(
        $.ajax(url + 'detections.json/settings', {
            type: 'POST'
          , contentType: 'application/json; charset=utf-8'
          , data: JSON.stringify(detectionSettings)
          , success: function (data, textStatus) {
              console.log('[success]', data, textStatus);
              $('#js-viewport').text(JSON.stringify(data, null, '  '));
            }
          , error: function (jqXhr, textStatus, err) {
              console.error('[error]', err, textStatus);
              $('#js-viewport').text(err);
            }
        })
      , $.ajax(url + 'tracks.json/settings', {
            type: 'POST'
          , contentType: 'application/json; charset=utf-8'
          , data: JSON.stringify(trackSettings)
          , success: function (data, textStatus) {
              console.log('[success]', data, textStatus);
              $('#js-viewport').text(JSON.stringify(data, null, '  '));
            }
          , error: function (jqXhr, textStatus, err) {
              console.error('[error]', err, textStatus);
              $('#js-viewport').text(err);
            }
        })
    ).done(function () {
      console.log('both done');
    });
  }

  $(function () {
    exitUnlessCompatible();
  });

}());
