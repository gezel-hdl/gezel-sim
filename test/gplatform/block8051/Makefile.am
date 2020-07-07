include ../../Makefile.cfg
include ../../sdcc.cfg

TARGET = ramrw.ihx

SDCCCFLAGS = --model-large

.PHONY: all
all: $(TARGET)

.PHONY: help
help:
	@echo "To simulate, type 'make sim'"  

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) -c 100000 blockio.fdl

.PHONY: clean-local
clean-local: clean-sdcc

EXTRA_DIST	=       \
    blockio.fdl     \
    ramrw.c
