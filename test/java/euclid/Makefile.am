# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $

include ../../Makefile.cfg
include ../../java.cfg

TARGET = euclid
TARGETS = $(TARGET).class

.PHONY: all
all: $(TARGETS)

.PHONY: sim
sim: $(TARGETS)
	$(JAVASIM) $(TARGET)

.PHONY: clean-local
clean-local: clean-java
	rm -f $(TARGETS)

EXTRA_DIST =        \
    euclid.fdl      \
    euclid.java
