include ../../Makefile.cfg
include ../../arm.cfg

TARGET = cycle.bin

all: $(TARGET)

.PHONY: sim
sim:
	$(GPLATFORM) cycle.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) *.s fdl.symbols *~

EXTRA_DIST=     \
    cycle.c     \
    cycle.fdl
