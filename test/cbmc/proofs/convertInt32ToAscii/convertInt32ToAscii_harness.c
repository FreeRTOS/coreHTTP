/*
 * coreHTTP
 * Copyright (C) 2024 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint8_t __CPROVER_file_local_core_http_client_c_convertInt32ToAscii( int32_t value,
                                                                    char * pBuffer,
                                                                    size_t bufferLength );

void convertInt32ToAscii_harness( void )
{
    static const char expected[] = "-2147483648";
    char buffer[ sizeof( expected ) ] = { 0 };
    uint8_t bytesWritten;

    bytesWritten = __CPROVER_file_local_core_http_client_c_convertInt32ToAscii( INT32_MIN,
                                                                                 buffer,
                                                                                 sizeof( buffer ) );

    assert( bytesWritten == ( sizeof( expected ) - 1U ) );
    assert( memcmp( buffer, expected, bytesWritten ) == 0 );
}
