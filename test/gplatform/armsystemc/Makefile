include ../../Makefile.cfg

# path to GEZEL installation
GEZEL=$(shell pwd)/../../../build
GCCOPTS=-O3

# path to Simit-ARM
ARMBUILD=/home/schaum/SimIt-ARM-2.1/build

# include & libraries: include SYSTEMC, Simit-ARM, and GEZEL
INCL=-I$(SYSTEMC)/include -I$(ARMBUILD)/include -I$(GEZEL)/include/gezel
LIB=-L$(SYSTEMC)/lib-linux/ -lsystemc -L$(ARMBUILD)/lib -larmsim -larmemu


# how to compile C++ files
%.o: %.cxx
	g++ -g $(GCCOPTS) -Wall -c $(INCL) $< -o $@


# accum_sc.cxx  = custom systemc cosimulator
# armsystem.cxx = arm cosimulation interfaces, used by systemc cosimulator
OBJS= armsystem.o accum_sc.o 

all: systemc_cosim

# how to compile C software for ARM
syscarmdriver: syscarmdriver.c
	$(ARMCC) -static $< -o $@


# cosimulator link command
systemc_cosim: syscarmdriver \
		$(OBJS) \
		$(GEZEL)/lib/libgzlsysc.a \
		$(GEZEL)/lib/libfdl.a \
		$(SYSTEMC)/lib-linux/libsystemc.a
	g++ -g $(OBJS) -L$(GEZEL)/lib/ -lgzlsysc -lfdl $(LIB) -lgmp -ldl -o systemc_cosim -Wl,--rpath -Wl,$(GEZEL)/lib/

sim:
	export LD_LIBRARY_PATH=../../build/lib; ./systemc_cosim

clean:
	rm -f syscarmdriver *.o *~ *.exe armzilla sysconnect.tab.* sysconnect.cxx systemc_cosim
