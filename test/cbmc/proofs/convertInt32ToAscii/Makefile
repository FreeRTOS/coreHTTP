HARNESS_ENTRY=convertInt32ToAscii_harness
HARNESS_FILE=$(HARNESS_ENTRY)
PROOF_UID=convertInt32ToAscii

UNWINDSET += __CPROVER_file_local_core_http_client_c_convertInt32ToAscii.0:11
UNWINDSET += __CPROVER_file_local_core_http_client_c_convertInt32ToAscii.1:11

PROJECT_SOURCES += $(SRCDIR)/source/core_http_client.c

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c

include ../Makefile.common
