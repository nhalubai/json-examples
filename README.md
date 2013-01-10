# UNTESTED CODE BRANCH

This code is intended to demonstrate the basics of JSON and HTTP interactions in each language present. It is provided as-is; see the [Master branch README.md](https://github.com/SpotterRF/json-examples/blob/master/README.md) for more detail.

# IMPORTANT

The parser you choose **MUST** be configureable meet the following criteria:

  * IGNORE unexpected and unused fields (new fields may be added at any time)
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

