/*jshint strict:true browser:true jquery:true es5:true onevar:true laxcomma:true laxbreak:true eqeqeq:true immed:true latedef:true undef:true unused:true*/
(function () {
  "use strict";

  var $ = window.jQuery
    ;

  $.get('http://cors.foobar3000.com/echo/example.json', function (data) {
    //console.log(data);
    $('#js-viewport').text(JSON.stringify(data, null, '  '));
  }, 'json');
}());
