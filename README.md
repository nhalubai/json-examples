# json-examples

Examples of JSON parsing / generation and http requests in various languages

# Solutions Explored

Demo code has been written in the following languages:

  0. Javascript
    * [jQuery](http://jquery.com) (in Browser, all the hard stuff is handled)
    * [NodeJS](http://nodejs.org) (Best HTTP and JSON support; server-side)
  0. [Ruby](http://ruby-lang.org)
  0. [Python](http://python.org)
  0. [luvit](http://luvit.io) (lua)
  0. C#
  0. Java
  0. curl (exec from any language)
  0. bash
  0. C

However, most of the code we provide is not very robust, so it is in the [**"untested"** branch of this repository](https://github.com/SpotterRF/json-examples/tree/untested). It is provided so the astute programmer can see the basics of normal JSON and HTTP interactions in the language, rather than as a complete, solid application off of which others may build.

Code that *is* tested and suitable as a starting point is in the [master branch](https://github.com/SpotterRF/json-examples). Languages that fall into this category are:

  0. C

Your mileage may vary. *Understand the code you are using before actually using it.*

ProTip: Use an Application Language (NodeJS, Python, Ruby) to do all of your HTTP / JSON work coupled with some sort of message passing (such as packing a struct) if you need to use a Systems Language (C, Java, C#).

In the future we hope to also have example code in these languages:

  0. [GoLang (or Go for short)](http://golang.org)

# IMPORTANT

The parser you choose **MUST** be configureable meet the following criteria:

  * IGNORE unexpected and unused fields (new fields will be added in future versions)
  * ACCEPT `null` as a non-value for any field type

Additionally:

  * when a number is encountered where a string was expected, the number should be treated as a string
  * if an expected field does not exist, it should be treated the same as if the value were `null`

# Examples

The eventual goal is to have examples for all of the following in each language:

  * HTTP GET request
  * HTTP POST request
  * JSON parsing (decoding / deserializing)
  * Iterating over objects and arrays
  * JSON stringifying (encoding / serializing)

# SDK

One day we'll have an SDK...
