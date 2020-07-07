include ../../Makefile.cfg
include ../../sdcc.cfg

TARGET = driver.ihx

.PHONY: all
all: $(TARGET)

.PHONY: help
help:
	@echo "To simulate, type 'make sim'"  

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) hello.fdl

.PHONY: clean-local
clean-local: clean-sdcc
	rm -f *~ *.asm *.lnk *.map *.rel *.sym *.rst *.mem *.ihx *.lst

EXTRA_DIST =    \
    driver.c    \
    hello.fdl
