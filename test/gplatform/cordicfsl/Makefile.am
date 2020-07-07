include ../../Makefile.cfg
include ../../arm.cfg

TARGETS = cordic_fixp.bin cordic_ref.bin

.PHONY: all
all: $(TARGETS)

ARMCFLAGS = -O3

cordic_ref.bin: cycle.o cordic_ref.o

.PHONY: sim_fixp
sim_fixp:
	$(GPLATFORM) fsl.fdl

.PHONY: sim_ref
sim_ref:
	$(GPLATFORM) fsl_ref.fdl

.PHONY: sim
sim: sim_fixp sim_ref

.PHONY: vhd
vhd:
	$(FDLVHD) -c FSL_Clk FSL_Rst fsl.fdl

.PHONY: clean-local
clean-local:
	rm -f *.o $(TARGETS) fdl.symbols *~

EXTRA_DIST =        \
    cordic_fixp.c   \
    cordic_ref.c    \
    cycle.s         \
    fsl.fdl         \
    fsl_ref.fdl
