include ../../Makefile.cfg
include ../../arm.cfg

TARGET = euclid.bin

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) euclid.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) *.s fdl.symbols *~

EXTRA_DIST =    \
    euclid.c    \
    euclid.fdl
