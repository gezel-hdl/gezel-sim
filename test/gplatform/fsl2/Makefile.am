include ../../Makefile.cfg
include ../../arm.cfg

TARGET = fsldrive.bin

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM)  fsl.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) *.s fdl.symbols *~

EXTRA_DIST =    \
    fsl.fdl     \
    fsldrive.c
