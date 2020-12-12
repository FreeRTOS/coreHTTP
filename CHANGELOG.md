# Changelog for coreHTTP Client Library

## v2.0.0 (December 2020)

### Updates

 - [#83](https://github.com/FreeRTOS/coreHTTP/pull/83) Implement transport send and receive timeouts in coreHTTP. Previously, a zero returned form the transport receive stopped the library from trying to receive more data. Now, the library can optionally retry transport receives, when zero is returned, up to a configured timeout. Also previously, the library had the potential to loop forever if the transport send returned zero indefinitely. Now, the library can optionally retry transport sends, when zero is returned, up to a configured timeout; if no timeout is configured, then sending will stop at the first zero returned. This change adds an optional callback to get the current timestamp in milliseconds. This change also adds configuration macros to set the timeout in milliseconds, for retrying the transport send and receive.
 - [#79](https://github.com/FreeRTOS/coreHTTP/pull/79), [#82](https://github.com/FreeRTOS/coreHTTP/pull/82) transport_interface.h documentation updates.
 - [#75](https://github.com/FreeRTOS/coreHTTP/pull/75) Small fix to cast logging arguments to types matching the format specifiers.

### Other
 - [#70](https://github.com/FreeRTOS/coreHTTP/pull/70), [#72](https://github.com/FreeRTOS/coreHTTP/pull/72), [#78](https://github.com/FreeRTOS/coreHTTP/pull/78) Github actions updates.
 - [#73](https://github.com/FreeRTOS/coreHTTP/pull/73), [#76](https://github.com/FreeRTOS/coreHTTP/pull/76) Github repo chores.
 - [#71](https://github.com/FreeRTOS/coreHTTP/pull/71) CBMC automation chore.
 - [#81](https://github.com/FreeRTOS/coreHTTP/pull/81), [#84](https://github.com/FreeRTOS/coreHTTP/pull/84) Doxygen memory estimates table update.

## v1.0.0 November 2020

This is the first release of the coreHTTP client library in this repository.

The HTTP client library is a client-side implementation that supports a subset
of the HTTP/1.1 protocol. It is optimized for resource-constrained devices, and
does not allocate any memory.
