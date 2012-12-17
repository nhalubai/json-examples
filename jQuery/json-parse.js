/*jshint strict:true browser:true jquery:true es5:true onevar:true laxcomma:true laxbreak:true eqeqeq:true immed:true latedef:true undef:true unused:true*/
(function () {
  "use strict";

  var json = {"foo":"bar","baz":["qux","quux"],"corge":{"grault":"garply","waldo":"fred","plugh":"xyzzy"},"thud":null}
    , data
    , pretty
    ;

  data = JSON.parse(json);
  console.log(data);

  pretty = JSON.stringify(data, null, '  ');
  console.log(pretty);
}());
