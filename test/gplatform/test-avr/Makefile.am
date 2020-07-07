include ../../Makefile.cfg
include ../../avr.cfg

TARGET = avr_check
TARGETS = $(TARGET).elf $(TARGET).lst $(TARGET).srec

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: sim_fixp

.PHONY: sim_fixp
sim_fixp: $(TARGET).srec
	$(GPLATFORM) avr_check.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) fdl.symbols *~ *.vhd $(TARGET).elf *.bak *.srec
	rm -f *.lst *.map $(EXTRA_CLEAN_FILES)

EXTRA_DIST =        \
    avr_check.c     \
    avr_check.fdl
