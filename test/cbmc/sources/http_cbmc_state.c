/*
 * coreHTTP v2.0.2
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
 * @file http_cbmc_state.c
 * @brief Implements the functions in http_cbmc_state.h.
 */

#include <stdlib.h>

#include "http_cbmc_state.h"

HTTPRequestHeaders_t * allocateHttpRequestHeaders( HTTPRequestHeaders_t * pRequestHeaders )
{
    if( pRequestHeaders == NULL )
    {
        pRequestHeaders = malloc( sizeof( HTTPRequestHeaders_t ) );
    }

    if( pRequestHeaders != NULL )
    {
        pRequestHeaders->pBuffer = malloc( pRequestHeaders->bufferLen );
    }

    return pRequestHeaders;
}

bool isValidHttpRequestHeaders( const HTTPRequestHeaders_t * pRequestHeaders )
{
    bool isValid = true;
    return isValid;
}

HTTPRequestInfo_t * allocateHttpRequestInfo( HTTPRequestInfo_t * pRequestInfo )
{
    if( pRequestInfo == NULL )
    {
        pRequestInfo = malloc( sizeof( HTTPRequestInfo_t ) );
    }

    if( pRequestInfo != NULL )
    {
        pRequestInfo->pMethod = malloc( pRequestInfo->methodLen );
        pRequestInfo->pHost = malloc( pRequestInfo->hostLen );
        pRequestInfo->pPath = malloc( pRequestInfo->pathLen );
    }

    return pRequestInfo;
}

bool isValidHttpRequestInfo( const HTTPRequestInfo_t * pRequestInfo )
{
    bool isValid = true;
    return isValid;
}

HTTPResponse_t * allocateHttpResponse( HTTPResponse_t * pResponse )
{
    size_t headerOffset, bodyOffset;

    if( pResponse == NULL )
    {
        pResponse = malloc( sizeof( HTTPResponse_t ) );
    }

    if( pResponse != NULL )
    {
        pResponse->pBuffer = malloc( pResponse->bufferLen );

        __CPROVER_assume( headerOffset <= pResponse->bufferLen );

        if( pResponse->pBuffer != NULL )
        {
            /* It is possible to have no headers in the response so set to NULL or
             * an offset in the response buffer. */
            pResponse->pHeaders = nondet_bool() ? NULL :
                                  pResponse->pBuffer + headerOffset;
        }

        if( pResponse->pHeaders != NULL )
        {
            /* The length of the headers MUST be between the start of the
             * headers and the end of the buffer. */
            __CPROVER_assume( pResponse->headersLen < ( pResponse->bufferLen - headerOffset ) );
        }

        if( pResponse->bufferLen == 0 )
        {
            bodyOffset = 0;
        }
        else
        {
            __CPROVER_assume( bodyOffset <= pResponse->bufferLen );
        }

        if( pResponse->pBuffer != NULL )
        {
            pResponse->pBody = nondet_bool() ? NULL :
                               pResponse->pBuffer + bodyOffset;
        }

        /* The length of the body MUST be between the start of body and the end
         * of the buffer. */
        if( pResponse->pBody )
        {
            __CPROVER_assume( pResponse->bodyLen < ( pResponse->bufferLen - bodyOffset ) );
        }
    }

    return pResponse;
}

bool isValidHttpResponse( const HTTPResponse_t * pResponse )
{
    bool isValid = true;

    if( pResponse )
    {
        isValid = ( pResponse->bodyLen < pResponse->bufferLen ) &&
                  ( pResponse->headersLen < pResponse->bufferLen );
        isValid = isValid || pResponse->pBody == NULL;
    }

    return isValid;
}

TransportInterface_t * allocateTransportInterface( TransportInterface_t * pTransport )
{
    if( pTransport == NULL )
    {
        pTransport = malloc( sizeof( TransportInterface_t ) );
    }

    if( pTransport != NULL )
    {
        pTransport->pNetworkContext = malloc( sizeof( NetworkContext_t ) );
    }

    return pTransport;
}

bool isValidTransportInterface( TransportInterface_t * pTransportInterface )
{
    bool isValid = true;

    if( pTransportInterface )
    {
        isValid = isValid && ( pTransportInterface->send == TransportInterfaceSendStub ||
                               pTransportInterface->send == NULL );
        isValid = isValid && ( pTransportInterface->recv == TransportInterfaceReceiveStub ||
                               pTransportInterface->recv == NULL );
    }
}

http_parser * allocateHttpSendParser( http_parser * pHttpParser )
{
    HTTPParsingContext_t * pHttpParsingContext;

    if( pHttpParser == NULL )
    {
        pHttpParser = malloc( sizeof( http_parser ) );
        __CPROVER_assume( pHttpParser != NULL );
    }

    pHttpParsingContext = allocateHttpSendParsingContext( NULL );
    __CPROVER_assume( isValidHttpSendParsingContext( pHttpParsingContext ) );
    pHttpParser->data = ( void * ) pHttpParsingContext;

    return pHttpParser;
}

HTTPParsingContext_t * allocateHttpSendParsingContext( HTTPParsingContext_t * pHttpParsingContext )
{
    HTTPResponse_t * pResponse;
    size_t bufferOffset;

    if( pHttpParsingContext == NULL )
    {
        pHttpParsingContext = malloc( sizeof( HTTPParsingContext_t ) );
        __CPROVER_assume( pHttpParsingContext != NULL );

        pResponse = allocateHttpResponse( NULL );
        __CPROVER_assume( isValidHttpResponse( pResponse ) &&
                          pResponse != NULL &&
                          pResponse->pBuffer != NULL &&
                          pResponse->bufferLen > 0 );
        pHttpParsingContext->pResponse = pResponse;

        __CPROVER_assume( bufferOffset < pResponse->bufferLen );
        pHttpParsingContext->pBufferCur = pResponse->pBuffer + bufferOffset;
    }

    return pHttpParsingContext;
}

bool isValidHttpSendParsingContext( const HTTPParsingContext_t * pHttpParsingContext )
{
    bool isValid = true;
    return isValid;
}

http_parser * allocateHttpReadHeaderParser( http_parser * pHttpParser )
{
    HTTPParsingContext_t * pFindHeaderContext;

    if( pHttpParser == NULL )
    {
        pHttpParser = malloc( sizeof( http_parser ) );
        __CPROVER_assume( pHttpParser != NULL );
    }

    pFindHeaderContext = allocateFindHeaderContext( NULL );
    pHttpParser->data = ( void * ) pFindHeaderContext;

    return pHttpParser;
}

findHeaderContext_t * allocateFindHeaderContext( findHeaderContext_t * pFindHeaderContext )
{
    if( pFindHeaderContext == NULL )
    {
        pFindHeaderContext = malloc( sizeof( findHeaderContext_t ) );
        __CPROVER_assume( pFindHeaderContext != NULL );
    }

    return pFindHeaderContext;
}
