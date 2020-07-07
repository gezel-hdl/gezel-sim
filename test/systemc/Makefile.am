#--------------------------------------------------------------
# (c) 2001-2003 University of California at Los Angeles
# Patrick Schaumont
#
# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $
#--------------------------------------------------------------

include ../Makefile.cfg

GEZEL=$(shell pwd)/../../build
GCCOPTS=-O3

#---------------------------------------------------------------

GZLINC=$(GEZEL)/include/gezel
GZLLIB=./lib

INCL=-I$(SYSTEMC)/include
LIB=-L$(SYSTEMC)/lib-linux/ -lsystemc

%.o: %.cxx
	g++ -g $(GCCOPTS) -Wall -c -I$(SYSTEMC)/include -I$(GEZEL)/include/gezel $< -o $@

OBJS= accum_sc.o

all: systemc_cosim

systemc_cosim: $(OBJS) $(GEZEL)/lib/libgzlsysc.a $(GEZEL)/lib/libfdl.a $(SYSTEMC)/lib-linux/libsystemc.a
	g++ -g $(OBJS) -L$(GEZEL)/lib/ -lgzlsysc -lfdl -L$(SYSTEMC)/lib-linux -lsystemc -lgmp -ldl -o systemc_cosim -Wl,--rpath -Wl,$(GEZEL)/lib/

sim:
	export LD_LIBRARY_PATH=../../build/lib; ./systemc_cosim

clean-local:
	rm -f *.o *~ *.exe armzilla sysconnect.tab.* sysconnect.cxx systemc_cosim

EXTRA_DIST =        \
    accum.fdl       \
    accum_sc.cxx
