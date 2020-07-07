
# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $

include ../Makefile.cfg

all:
	@echo "To simulate, select one of the following targets:"
	@echo "make bresen, euclid, gf8inv, ipchecksum, aes, ramblock, lfsr"
	@echo "For scripted.fdl, refer to the comments in that file directly"

sim: bresen euclid gf8inv ipchecksum ramblock aes lfsr toggle

bresen:
	$(FDLSIM) bresen.fdl 20

euclid:
	$(FDLSIM) euclid.fdl 30

gf8inv:
	$(FDLSIM) gf8inv.fdl 20

ipchecksum:
	$(FDLSIM) ipchecksum.fdl 200

ramblock:
	$(FDLSIM) ramblock.fdl 30

ramblock2:
	$(FDLSIM) ramblock2.fdl 30

aes:
	$(FDLSIM) aes.fdl 50

lfsr:
	$(FDLSIM) lfsr.fdl 20

toggle:
	$(FDLSIM) toggle.fdl 20
