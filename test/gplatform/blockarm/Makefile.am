include ../../Makefile.cfg
include ../../arm.cfg

TARGET = ramrw.bin

.PHONY: all
all: $(TARGET)

ramrw.bin: cycle.o ramrw.o

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) blockio.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) fdl.symbols *~

EXTRA_DIST =    \
    blockio.fdl \
    cycle.s     \
    ramrw.c
