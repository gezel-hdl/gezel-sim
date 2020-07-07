include ../../Makefile.cfg
include ../../arm.cfg

TARGETS = writer.bin reader.bin

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) intf.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGETS) *.s fdl.symbols *~

EXTRA_DIST = 	\
    intf.fdl    \
    reader.c    \
    writer.c
