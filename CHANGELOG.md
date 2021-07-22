# Changelog for coreHTTP Client Library

## v2.0.2 (July 2021)

### Updates

- [#109](https://github.com/FreeRTOS/coreHTTP/pull/109) Add C++ header guards
- [#106](https://github.com/FreeRTOS/coreHTTP/pull/106) Update case-insensitive compare function for header-field parser
- [#104](https://github.com/FreeRTOS/coreHTTP/pull/104) Update CBMC proofs to work with the latest version of CBMC

## v2.0.1 (February 2021)

### Other

- [#89](https://github.com/FreeRTOS/coreHTTP/pull/89) Fix documentation of memory size estimates of the library.

## v2.0.0 (December 2020)

### Updates

 - [#83](https://github.com/FreeRTOS/coreHTTP/pull/83) Implement transport send and receive retry timeouts in coreHTTP. This change adds a timestamp callback function to the HTTPResponse_t struct, and new configuration macros to set the transport send and receive retry timeouts. Due to the HTTPResponse_t struct field addition, coreHTTP v2.0.0 is not backward compatible under certain conditions.
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
