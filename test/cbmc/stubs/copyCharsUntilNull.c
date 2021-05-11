/*
 * coreHTTP v2.0.1
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
 * @file copyCharsUntilNull.c
 * @brief Creates a stub for copyCharsUntilNull so that the proofs for
 * HTTPClient_AddHeader, HTTPClient_AddRangeHeader, and
 * HTTPClient_InitializeRequestHeaders run much faster. This stub checks if, for
 * the input copy length, the destination and source are valid accessible
 * memory.
 */

#include <string.h>
#include <stdint.h>

size_t copyCharsUntilNull( char * pDest,
                           const char * pSrc,
                           size_t maxLen )
{
    __CPROVER_assert( __CPROVER_w_ok( pDest, maxLen ), "write" );
    __CPROVER_assert( __CPROVER_r_ok( pSrc, maxLen ), "read" );
    return maxLen;
}
