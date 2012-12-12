# json-examples

Examples of JSON parsing / generation and http requests in various languages

# IMPORTANT

The parser you choose **MUST** be configureable meet the following criteria:

  * IGNORE unexpected and unused fields (new fields may be added at any time)
  * ACCEPT `null` as a non-value for any field type

Additionally:

  * when a number is encountered where a string was expected, the number should be treated as a string
  * if an expected field does not exist, it should be treated the same as if the value were `null`
