include ../../Makefile.cfg
include ../../arm.cfg

TARGETS = sfudriver.bin

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) armsfu.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGETS) *.s fdl.symbols *~

EXTRA_DIST =    \
    arm_ext.h   \
    armsfu.fdl  \
    sfudriver.c
