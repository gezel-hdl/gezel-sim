include ../Makefile.cfg

.PHONY: help
help:
	@echo "To simulate, select one of the following targets:"
	@echo "make bresen, euclid, gf8inv, ipchecksum, aes, ramblock, lfsr"
	@echo "For scripted.fdl, refer to the comments in that file directly"

.PHONY: sim
sim: bresen euclid gf8inv ipchecksum ramblock aes lfsr toggle

.PHONY: bresen
bresen:
	$(FDLSIM) bresen.fdl 20

.PHONY: euclid
euclid:
	$(FDLSIM) euclid.fdl 30

.PHONY: gf8inv
gf8inv:
	$(FDLSIM) gf8inv.fdl 20

.PHONY: ipchecksum
ipchecksum:
	$(FDLSIM) ipchecksum.fdl 200

.PHONY: ramblock
ramblock:
	$(FDLSIM) ramblock.fdl 30

.PHONY: ramblock2
ramblock2:
	$(FDLSIM) ramblock2.fdl 30

.PHONY: aes
aes:
	$(FDLSIM) aes.fdl 50

.PHONY: lfsr
lfsr:
	$(FDLSIM) lfsr.fdl 20

.PHONY: scripted
scripted:
	$(builddir)/toggle.fdl 20

.PHONY: toggle
toggle:
	$(FDLSIM) toggle.fdl 20

EXTRA_DIST =         \
    aes.fdl          \
    bresen.fdl       \
    euclid.fdl       \
    gf8inv.fdl       \
    ipchecksum.fdl   \
    lfsr.fdl         \
    ramblock.fdl     \
    ramblock2.fdl    \
    scripted.fdl.in  \
    toggle.fdl
