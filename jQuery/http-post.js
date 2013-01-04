/*jshint strict:true browser:true jquery:true es5:true onevar:true laxcomma:true laxbreak:true eqeqeq:true immed:true latedef:true undef:true unused:true*/
(function () {
  "use strict";

  var $ = window.jQuery
    ;

  $.ajax('http://cors.foobar3000.com/echo/example.json', {
      // 'type' is a historical misnomer
      // what it means is 'method'
      type: 'POST'
      // contentType refers to the uploaded data
    , contentType: 'application/json; charset=utf-8'
      // 'data' is a historical misnomer
      // what it means is 'body'
    , data: JSON.stringify({
          foo: 'bar'
        , baz: ['qux', 'quux']
        , corge: {
              grault: 'garply'
            , waldo: 'fred'
            , plugh: 'xyzzy'
          }
        , thud: null
      })
    , complete: function (jqXhr, textStatus) {
        //console.log('completed (whether success or failure)');
      }
    , success: function (data, textStatus) {
        //console.log(data);
        $('#js-viewport').text(JSON.stringify(data, null, '  '));
      }
    , error: function (jqXhr, textStatus, err) {
        //console.error(err);
        $('#js-viewport').text(err);
      }
  });
}());
