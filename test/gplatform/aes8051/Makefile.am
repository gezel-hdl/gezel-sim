include ../../Makefile.cfg
include ../../sdcc.cfg

.PHONY:help
help:
	@echo "To simulate, type 'make sim'"

TARGETS = driver.ihx

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) aes.fdl

CLEANFILES: driver.ihx

.PHONY: clean-local
clean-local: clean-sdcc

EXTRA_DIST =    \
    driver.c    \
    aes.fdl
