include ../../Makefile.cfg

# FIXME: pass variables through autoconf:
# FIXME: simit -> SIMIT_INCLUDES SIMIT_LIBS(armsim armemu)
# FIXME: systemc -> SYSTEMC_INCLUDES SYSTEMC_LIBS(systemc)

# path to GEZEL installation
#GEZEL=$(shell pwd)/../../../build
#GCCOPTS=-O3

# path to Simit-ARM
#ARMBUILD=/home/schaum/SimIt-ARM-2.1/build

# include & libraries: include SYSTEMC, Simit-ARM, and GEZEL
#INCLUDES = -I$(SYSTEMC)/include -I$(ARMBUILD)/include -I$(GEZEL)/include/gezel
#LIB=-L$(SYSTEMC)/lib-linux/ -lsystemc -L$(ARMBUILD)/lib -larmsim -larmemu

# accum_sc.cxx  = custom systemc cosimulator
# armsystem.cxx = arm cosimulation interfaces, used by systemc cosimulator

AM_CXXFLAGS = -I$(top_srcdir)/gezel $(SIMIT_INCLUDES) $(SYSTEMC_INCLUDES)

TARGETS = systemc_cosim syscarmdriver

.PHONY: all
all: $(TARGETS)

syscarmdriver: syscarmdriver.c
	$(ARMCC) -static $< -o $@

noinst_PROGRAMS = systemc_cosim
systemc_cosim_SOURCES = armsystem.cxx accum_sc.cxx
systemc_cosim_LDADD = $(top_builddir)/systemc/gzlsysc.la
systemc_cosim_LDFLAGS = $(SIMIT_LIBS) $(SYSTEMC_LIBS)

.PHONY: sim
sim: $(TARGETS)
	LD_LIBRARY_PATH=../../build/lib ./systemc_cosim$(EXEEXT)

.PHONY: clean-local
clean-local:
	rm -f syscarmdriver *.o *~ *.exe armzilla sysconnect.tab.* sysconnect.cxx systemc_cosim

EXTRA_DIST = 		\
    accum.fdl       \
    accum_sc.cxx    \
    armsystem.cxx   \
    syscarmdriver.c
