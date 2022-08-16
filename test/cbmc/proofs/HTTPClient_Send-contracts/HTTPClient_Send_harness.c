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
 * @file HTTPClient_Send_harness.c
 * @brief Implements the proof harness for HTTPClient_Send function.
 */

#include "core_http_client.h"
#include "http_cbmc_state.h"
#include "transport_interface_stubs.h"
#include "get_time_stub.h"

// clang-format off
int32_t contract_tries = 0;

// Assigns clause for transport interface send callbacks
__CPROVER_assignable_t TransportInterfaceSendAssigns() {
    __CPROVER_typed_target(contract_tries);
}

// Contract for transport interface send callbacks
int32_t TransportInterfaceSendContract(
    NetworkContext_t * pNetworkContext,
    void * pBuffer,
    size_t bytesToSend
)
__CPROVER_requires(pBuffer!=NULL)
__CPROVER_assigns(TransportInterfaceSendAssigns())
__CPROVER_ensures(
    (bytesToSend <= INT32_MAX) ==> 
    (__CPROVER_return_value <= (int32_t)bytesToSend))
__CPROVER_ensures(
    __CPROVER_old(contract_tries) >= MAX_TRIES ?
    (contract_tries == 0 && __CPROVER_return_value == -1) :
    (contract_tries == __CPROVER_old(contract_tries) + 1))
;   

// Assigns clause for transport interface receive callbacks
__CPROVER_assignable_t TransportInterfaceRecvAssigns() { }

// Contract for transport interface receive callbacks
int32_t TransportInterfaceRecvContract(
    NetworkContext_t *pNetworkContext,
    void *pBuffer,
    size_t bytesToRecv)
__CPROVER_requires(pBuffer!=NULL)
__CPROVER_assigns(TransportInterfaceRecvAssigns())
__CPROVER_ensures(
    (bytesToRecv <= INT32_MAX) ==> 
    (__CPROVER_return_value <= (int32_t)bytesToRecv))
;

uint32_t contract_globalEntryTime = 0;

// Assigns clause for get time callbacks
__CPROVER_assignable_t GetCurrentTimeAssigns( void ) {
    __CPROVER_typed_target(contract_globalEntryTime);
}

// Contract for get time callbacks
uint32_t GetCurrentTimeContract( void )
__CPROVER_requires(1)
__CPROVER_assigns(GetCurrentTimeAssigns())
__CPROVER_ensures(contract_globalEntryTime == __CPROVER_old(contract_globalEntryTime) + 1)
;


// Assigns clause for HTTPClient_Send
__CPROVER_assignable_t HTTPClient_SendAssigns(
    const TransportInterface_t *pTransport,
    HTTPRequestHeaders_t *pRequestHeaders,
    const uint8_t *pRequestBodyBuf,
    size_t reqBodyBufLen,
    HTTPResponse_t *pResponse,
    uint32_t sendFlags)
{
  if (pRequestHeaders != NULL) {
    __CPROVER_typed_target(pRequestHeaders->headersLen);
    if (pRequestHeaders->pBuffer != NULL) {
        __CPROVER_object_from(pRequestHeaders->pBuffer);
    }
  }

  if (pResponse != NULL) {
    __CPROVER_typed_target(pResponse->getTime);
    __CPROVER_typed_target(pResponse->statusCode);
    __CPROVER_typed_target(pResponse->pBody);
    __CPROVER_typed_target(pResponse->bodyLen);
    __CPROVER_typed_target(pResponse->pHeaders);
    __CPROVER_typed_target(pResponse->headersLen);
    __CPROVER_typed_target(pResponse->headerCount);
    __CPROVER_typed_target(pResponse->respFlags);
  }
}

// Contract for HTTPClient_Send
HTTPStatus_t HTTPClient_Send(
    const TransportInterface_t *pTransport,
    HTTPRequestHeaders_t *pRequestHeaders,
    const uint8_t *pRequestBodyBuf,
    size_t reqBodyBufLen,
    HTTPResponse_t *pResponse,
    uint32_t sendFlags)
__CPROVER_assigns(
    HTTPClient_SendAssigns(
            pTransport,
            pRequestHeaders,
            pRequestBodyBuf,
            reqBodyBufLen,
            pResponse,
            sendFlags);
    // assign whatever the callbacks assign
    TransportInterfaceSendAssigns();
    TransportInterfaceRecvAssigns();
    GetCurrentTimeAssigns();
)
// function pointers assumptions
__CPROVER_requires(pTransport != NULL)
__CPROVER_requires_contract(pTransport->send, TransportInterfaceSendContract)
__CPROVER_requires_contract(pTransport->recv, TransportInterfaceRecvContract)
__CPROVER_requires(pResponse->getTime != NULL)
__CPROVER_requires_contract(pResponse->getTime, GetCurrentTimeContract)
;
// clang-format on

void HTTPClient_Send_harness()
{
    HTTPRequestHeaders_t * pRequestHeaders = NULL;
    HTTPResponse_t * pResponse = NULL;
    TransportInterface_t * pTransportInterface = NULL;
    uint8_t * pRequestBodyBuf = NULL;
    size_t reqBodyBufLen;
    uint32_t sendFlags;

    /* Initialize and make assumptions for request headers. */
    pRequestHeaders = allocateHttpRequestHeaders( NULL );
    __CPROVER_assume( isValidHttpRequestHeaders( pRequestHeaders ) );

    /* Initialize and make assumptions for buffer to receive request body. */
    __CPROVER_assume( reqBodyBufLen < CBMC_MAX_OBJECT_SIZE );
    pRequestBodyBuf = mallocCanFail( reqBodyBufLen );

    /* Initialize and make assumptions for response object. */
    pResponse = allocateHttpResponse( NULL );
    __CPROVER_assume( isValidHttpResponse( pResponse ) );

    /* Initialize transport interface. */
    pTransportInterface = allocateTransportInterface( NULL );

    HTTPClient_Send( pTransportInterface,
                     pRequestHeaders,
                     pRequestBodyBuf,
                     reqBodyBufLen,
                     pResponse,
                     sendFlags );
}
