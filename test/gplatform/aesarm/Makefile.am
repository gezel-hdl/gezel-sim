include ../../Makefile.cfg
include ../../arm.cfg

TARGETS = aes_coproc_armdriver.bin

.PHONY: all
all: $(TARGETS)

#aes_coproc_armdriver: aes_coproc_armdriver.o
#	$(ARMCC) -static $< -o $@

.PHONY: sim
sim: $(TARGETS)
	$(GPLATFORM) aes_coproc_arm.fdl

.PHONY: clean-local
clean-local:
	rm -f $(TARGETS)

EXTRA_DIST =                \
	aes_coproc_armdriver.c  \
	aes_coproc_arm.fdl
