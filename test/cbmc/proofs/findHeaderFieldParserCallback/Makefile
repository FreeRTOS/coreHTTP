# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

HARNESS_ENTRY=findHeaderFieldParserCallback_harness
PROOF_UID=findHeaderFieldParserCallback
HARNESS_FILE=$(HARNESS_ENTRY)

# The header field length is bounded, so strncmp can be unwound to an expected
# amount that won't make the proof run too long.
MAX_HEADER_FIELD_LENGTH=10

DEFINES += -DMAX_HEADER_FIELD_LENGTH=$(MAX_HEADER_FIELD_LENGTH)
INCLUDES +=

REMOVE_FUNCTION_BODY +=
UNWINDSET += strncmp.0:$(MAX_HEADER_FIELD_LENGTH)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/http_cbmc_state.c

PROJECT_SOURCES += $(SRCDIR)/source/core_http_client.c

include ../Makefile.common
