include ../../Makefile.cfg
include ../../arm.cfg

TARGET = hello.bin

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(TARGET)
	$(GPLATFORM) hellomodel.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGET) *.s fdl.symbols *~

EXTRA_DIST =        \
    hello.c         \
    hellomodel.fdl
