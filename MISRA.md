# MISRA Compliance

The HTTP Client library files conform to the [MISRA C:2012](https://www.misra.org.uk)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
Deviations from the MISRA standard are listed below. The deviations below do not
include the third-party [http-parser source code](https://github.com/nodejs/http-parser/tree/v2.9.3):

### Ignored by [Coverity Configuration](https://github.com/aws/aws-iot-device-sdk-embedded-C/blob/main/tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-- |
| Directive 4.5 | Advisory | Allow names that MISRA considers ambiguous (such as LogInfo and LogError). |
| Directive 4.8 | Advisory | Allow inclusion of unused types. Header files for a specific port, which are needed by all files, may define types that are not used by a specific file. |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. The `assert` macro is used throughout the library for parameter validation, and logging is done using function like macros. |
| Rule 2.4 | Advisory | Allow unused tags. Some compilers warn if types are not tagged. |
| Rule 2.5 | Advisory | Allow unused macros. Library headers may define macros intended for the application's use, but are not used by a specific file. |
| Rule 3.1 | Required | Allow nested comments. C++ style `//` comments are used in example code within Doxygen documentation blocks. |
| Rule 11.5 | Advisory | Allow casts from `void *`. The third-party http-parser library callback contexts are saved as `void *` and must be cast to the correct data type before use. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-- |
| Directive 4.6 | Advisory | The third-party http-parser library does not use specific-length typedefs for their callback function signatures and public structure fields. http-parser callbacks are implemented in the HTTP Client source and also flags of basic numerical types are checked from the http-parser structure.
| Rule 8.7 | Advisory | API functions are not used by the library outside of the files they are defined; however, they must be externally visible in order to be used by an application. |
| Rule 8.13 | Advisory | The third-party http-parser library callback definitions have a non-const parameter which holds the state of the parsing. This parameter is never updated in the callback implementations, but have fields that may be read. |
| Rule 10.5 | Advisory | The third-party http-parser library has a structure with a field of type unsigned int whose values are intended to be mapped to an enum. This field contains error codes used by the HTTP client library. |
| Rule 14.3 | Required | The third-party http-parser library sets a uint64_t type field to `ULLONG_MAX` or `( ( uint64_t ) -1 )`, during its internal parsing. Coverity MISRA does not detect that this variable changes. This field is checked by the HTTP Client library. |

### Suppressed with Coverity Comments
| Deviation | Category | Justification |
| :-: | :-: | :-- |
| Rule 5.4 | Required | The length of string literal macro identifiers are labeled with the identifier of the string literal itself postfixed with "_LEN" for clarity. This is consistent throughout the library. |
| Rule 10.8 | Required | The size of the headers is found by taking the current location being parsed and subtracting it from the start of the headers. The start of the headers is set on the first header field found from http-parser. This always comes before finding the header length; if it does not, an assertion is triggered. |
| Rule 11.8 | Required | If the response body uses chunked transfer encoding, then it is necessary to copy over the chunk headers with the data to which the current parsing location points. The current parsing location is returned to the callback implementation as a `const char *`. |
| Rule 18.3 | Required | It is expected that the current location http-parser passes into the body parsing callback points to the same user response buffer; if it does not, an assertion is triggered. |
