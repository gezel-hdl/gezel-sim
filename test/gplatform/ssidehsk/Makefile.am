include ../../Makefile.cfg
include ../../arm.cfg

TARGETS = sender.bin receiver.bin

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) network.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGETS) *.s fdl.symbols *~

EXTRA_DIST =    \
    network.fdl \
    receiver.c  \
    sender.c
