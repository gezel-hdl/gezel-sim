include ../../Makefile.cfg

TARGETS = slave51.ihx drivearm.bin

.PHONY: all
all: $(TARGETS)

slave51.ihx: slave51.c
	$(SDCC) $(SDCCCFLAGS) $^ -o $@

drivearm.bin: drivearm.c
	$(ARMCC) $(ARMCFLAGS) $(ARMLDFLAGS) $^ -o $@

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) mpsoc.fdl

.PHONY: clean-local
clean-local: clean-sdcc
	rm -f $(TARGETS) *~

EXTRA_DIST =    \
    drivearm.c  \
    mpsoc.fdl   \
    slave51.c
