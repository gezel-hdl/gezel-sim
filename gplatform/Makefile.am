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

AM_CPPFLAGS = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock $(SIMITDEF)

bin_PROGRAMS = gplatform

gplatform_SOURCES =     \
    platform.cxx        \
    i8051sim.cxx        \
    i8051system.cxx     \
    armsystem.cxx       \
    xilinxitf.cxx       \
    armsystem.h         \
    xilinxitf.h         \
    i8051sim.h          \
    i8051system.h       \
    cpicoblaze.cxx      \
    cpicoblaze.h        \
    cpicoinstr.cxx      \
    cpicoinstr.h        \
    platform.h          \
    picoblaze.cxx       \
    picoblaze.h         \
    gezelavr.cxx        \
    gezelavr.h

gplatform_LDADD = ../ipblock/libipconfig.la ../gezel/libfdl.la
gplatform_LDFLAGS = -larmsim -larmemu -lsim -lbfd $(LDDYNLIB)
