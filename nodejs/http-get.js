/*jshint strict:true node:true es5:true onevar:true laxcomma:true laxbreak:true eqeqeq:true immed:true latedef:true undef:true unused:true*/
(function () {
  "use strict";

  var http = require('http')
    ;

  http.get('http://foobar3000.com/echo/example.json', function (response) {
    var chunks = []
      ;

    response.on('data', function (chunk) {
      chunks.push(chunk);
    });
    response.on('end', function () {
      var data = Buffer.concat(chunks)
        ;

      console.log(data.toString('utf8'));
    });
  });

}());
