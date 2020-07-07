include ../../Makefile.cfg
include ../../avr.cfg

TARGET = samplecode
TARGETS = $(TARGET).elf $(TARGET).lst $(TARGET).srec

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: sim_fixp

.PHONY: sim_fixp
sim_fixp: $(TARGET).srec
	$(GPLATFORM) -c 5000 samplecode.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGETS) fdl.symbols *~ *.vhd $(TARGET).elf *.bak *.srec
	rm -f *.lst *.map $(EXTRA_CLEAN_FILES)

EXTRA_DIST =        \
    samplecode.c    \
    samplecode.fdl
