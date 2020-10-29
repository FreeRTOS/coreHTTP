#
# Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

HARNESS_ENTRY=HTTPClient_AddHeader_harness
PROOF_UID=HTTPClient_AddHeader
HARNESS_FILE=$(HARNESS_ENTRY)

# The header field and value string lengths input into HTTPClient_AddHeader are
# bounded to reduce the proof run time. This length is a bound on httpHeaderStrncpy
# used to write the header field and values to the request header buffer. Memory
# safety on the request header buffer can be proven in a reasonable bound. It
# adds no value to the proof to write a header field and a header value of
# SIZE_MAX length.
HEADER_FIELD_MAX_LENGTH=16
HEADER_VALUE_MAX_LENGTH=16

DEFINES += -DHEADER_FIELD_MAX_LENGTH=$(HEADER_FIELD_MAX_LENGTH)
DEFINES += -DHEADER_VALUE_MAX_LENGTH=$(HEADER_VALUE_MAX_LENGTH)
INCLUDES +=

# We remove these function bodies so that they aren't categorized as possible
# function pointer targets, leading to faster execution time.
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_findHeaderFieldParserCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_findHeaderValueParserCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnBodyCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnHeaderFieldCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnHeaderValueCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnStatusCallback

REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnHeadersCompleteCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnMessageBeginCallback
REMOVE_FUNCTION_BODY += __CPROVER_file_local_core_http_client_c_httpParserOnMessageCompleteCallback

# strncmp is used to find if there exists "\r\n\r\n" at the end of the header
# buffer. Therefore, we need to unwind strncmp the length of "\r\n\r\n" + 1.
UNWINDSET += strncmp.0:5
# The longest string input into strncpy in addHeader() is "\r\n\r\n".
UNWINDSET += strncpy.0:5
UNWINDSET += __CPROVER_file_local_core_http_client_c_httpHeaderStrncpy.0:$(HEADER_FIELD_MAX_LENGTH)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/http_cbmc_state.c

PROJECT_SOURCES += $(SRCDIR)/source/core_http_client.c

include ../Makefile.common
