include ../../Makefile.cfg
include ../../arm.cfg

TARGET = ramprobe.bin

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) ramprobe.fdl

.PHONY: clean-local
clean-local:
	rm -f $(TARGET) *.o *~

EXTRA_DIST =        \
    ramprobe.c      \
    ramprobe.fdl
