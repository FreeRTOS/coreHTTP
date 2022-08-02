/*
 * coreHTTP v2.1.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file core_http_client_private.h
 * @brief Internal definitions to the HTTP Client library.
 */

#ifndef CORE_HTTP_CLIENT_PRIVATE_H_
#define CORE_HTTP_CLIENT_PRIVATE_H_

/**
 * @cond DOXYGEN_IGNORE
 * http-parser defaults this to 1, llhttp to 0.
 */
#ifndef LLHTTP_STRICT_MODE
    #define LLHTTP_STRICT_MODE    0
#endif
/** @endcond */

/* Third-party llhttp include. */
#include "llhttp.h"

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/**
 * @brief Preprocessor macro to calculate the length of a const char * string.
 * @note It is intended that this function-like macro be evaluated by the c
 *  preprocessor to the actual length of the constant string argument.
 */
#define CONST_STRLEN( string )    ( sizeof( string ) - 1U )

/**
 * @brief Indicator for function #httpHeaderStrncpy that the pSrc parameter is a
 * header value.
 */
#define HTTP_HEADER_STRNCPY_IS_VALUE      0U

/**
 * @brief Indicator for function #httpHeaderStrncpy that the pSrc parameter is a
 * header field.
 */
#define HTTP_HEADER_STRNCPY_IS_FIELD      1U

/**
 * @brief Maximum value of a 32 bit signed integer is 2,147,483,647.
 *
 * Used for calculating buffer space for ASCII representation of range values.
 */
#define MAX_INT32_NO_OF_DECIMAL_DIGITS    10U

/**
 * @brief Maximum buffer space for storing a Range Request Value.
 *
 * The largest Range Request value is of the form:
 * "bytes=<Max-Integer-Value>-<Max-Integer-Value>"
 */
#define HTTP_MAX_RANGE_REQUEST_VALUE_LEN                         \
    ( sizeof( "bytes=" ) - 1U + MAX_INT32_NO_OF_DECIMAL_DIGITS + \
      1U + MAX_INT32_NO_OF_DECIMAL_DIGITS )

/**
 * @brief Return value for llhttp registered callback to signal
 * continuation of HTTP response parsing. Equal to HPE_OK.
 */
#define LLHTTP_CONTINUE_PARSING             0

/**
 * @brief Return value for llhttp registered callback to signal halting
 * further execution.
 */
#define LLHTTP_STOP_PARSING                 HPE_USER

/**
 * @brief Return value for llhttp_t.on_headers_complete to signal
 * that the HTTP response has no body and to halt further execution.
 */
#define LLHTTP_STOP_PARSING_NO_BODY         1

/**
 * @brief Return value for llhttp_t.on_headers_complete to signal
 * halting further execution. This is the same return value that
 * indicates the HTTP response has no body, but unlike the -1 error
 * code, gives consistent return values for llhttp_execute in both
 * strict and non-strict modes.
 */
#define LLHTTP_STOP_PARSING_NO_HEADER       1

/**
 * @brief The minimum request-line in the headers has a possible one character
 * custom method and a single forward / or asterisk * for the path:
 *
 * @code
 * <1 character custom method> <1 character / or *> HTTP/1.x\r\n\r\n
 * @endcode
 *
 * Therefore the minimum length is 16. If this minimum request-line is not
 * satisfied, then the request headers to send are invalid.
 *
 * Note that custom methods are allowed per:
 * https://tools.ietf.org/html/rfc2616#section-5.1.1.
 */
#define HTTP_MINIMUM_REQUEST_LINE_LENGTH    16u

/**
 * @brief The state of the response message parsed after function
 * #parseHttpResponse returns.
 */
typedef enum HTTPParsingState_t
{
    HTTP_PARSING_NONE = 0,   /**< The parser has not started reading any response. */
    HTTP_PARSING_INCOMPLETE, /**< The parser found a partial reponse. */
    HTTP_PARSING_COMPLETE    /**< The parser found the entire response. */
} HTTPParsingState_t;

/**
 * @brief An aggregator that represents the user-provided parameters to the
 * #HTTPClient_ReadHeader API function. This will be used as context parameter
 * for the parsing callbacks used by the API function.
 */
typedef struct findHeaderContext
{
    const char * pField;     /**< The field that is being searched for. */
    size_t fieldLen;         /**< The length of pField. */
    const char ** pValueLoc; /**< The location of the value found in the buffer. */
    size_t * pValueLen;      /**< the length of the value found. */
    uint8_t fieldFound;      /**< Indicates that the header field was found during parsing. */
    uint8_t valueFound;      /**< Indicates that the header value was found during parsing. */
} findHeaderContext_t;

/**
 * @brief The HTTP response parsing context for a response fresh from the
 * server. This context is passed into the http-parser registered callbacks.
 * The registered callbacks are private functions of the form
 * httpParserXXXXCallbacks().
 *
 * The transitions of the httpParserXXXXCallback() functions are shown below.
 * The  XXXX is replaced by the strings in the state boxes:
 *
 * +---------------------+
 * |onMessageBegin       |
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onStatus             |
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onHeaderField        +<---+
 * +--------+------------+    |
 *          |                 |
 *          |                 |(More headers)
 *          |                 |
 *          v                 |
 * +--------+------------+    |
 * |onHeaderValue        +----^
 * +--------+------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onHeadersComplete    |
 * +---------------------+
 *          |
 *          |
 *          |
 *          v
 * +--------+------------+
 * |onBody               +<---+
 * +--------+--------+---+    |
 *          |        |        |(Transfer-encoding chunked body)
 *          |        |        |
 *          |        +--------+
 *          |
 *          v
 * +--------+------------+
 * |onMessageComplete    |
 * +---------------------+
 */
typedef struct HTTPParsingContext
{
    llhttp_t llhttpParser;            /**< Third-party llhttp context. */
    llhttp_settings_t llhttpSettings; /**< Third-party parser settings. */
    HTTPParsingState_t state;         /**< The current state of the HTTP response parsed. */
    HTTPResponse_t * pResponse;       /**< HTTP response associated with this parsing context. */
    uint8_t isHeadResponse;           /**< HTTP response is for a HEAD request. */

    const char * pBufferCur;          /**< The current location of the parser in the response buffer. */
    const char * pLastHeaderField;    /**< Holds the last part of the header field parsed. */
    size_t lastHeaderFieldLen;        /**< The length of the last header field parsed. */
    const char * pLastHeaderValue;    /**< Holds the last part of the header value parsed. */
    size_t lastHeaderValueLen;        /**< The length of the last value field parsed. */
} HTTPParsingContext_t;

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_HTTP_CLIENT_PRIVATE_H_ */
