# Changelog for coreHTTP Client Library

## v1.0.1 December 2020

### Updates

 - [#75](https://github.com/aws/coreHTTP/pull/75) Cast variable type based on format identifier to fix logging warnings.
 - [#79](https://github.com/aws/coreHTTP/pull/79) Update transport_interface.h to match coreMQTT.

### Other
 - [#70](https://github.com/aws/coreHTTP/pull/70), [#72](https://github.com/aws/coreHTTP/pull/72), [#78](https://github.com/aws/coreHTTP/pull/78) Github actions updates.
 - [#73](https://github.com/aws/coreHTTP/pull/73), [#76](https://github.com/aws/coreHTTP/pull/76) Github repo chores.
 - [#71](https://github.com/aws/coreHTTP/pull/71) CBMC automation update.

## v1.0.0 November 2020

This is the first release of the coreHTTP client library in this repository.

The HTTP client library is a client-side implementation that supports a subset
of the HTTP/1.1 protocol. It is optimized for resource-constrained devices, and
does not allocate any memory.
