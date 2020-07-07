bin_PROGRAMS = gplatform
gplatform_SOURCES = platform.cxx  i8051sim.cxx i8051system.cxx armsystem.cxx xilinxitf.cxx armsystem.h xilinxitf.h i8051sim.h  i8051system.h  cpicoblaze.cxx cpicoblaze.h cpicoinstr.cxx cpicoinstr.h platform.h picoblaze.cxx picoblaze.h

if LINKLIBDL
  LDDYNLIB = -ldl
else
  LDDYNLIB =
endif

if SUPPORTSIMITSFU
  SIMITDEF = -DSUPPORTSIMITSFU
else
  SIMITDEF =
endif

gplatform_LDADD = -lgmp -larmsim -larmemu -L$(top_builddir)/ipblock -lipconfig -L$(top_builddir)/gezel -lfdl $(LDDYNLIB)
INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock $(SIMITDEF)
