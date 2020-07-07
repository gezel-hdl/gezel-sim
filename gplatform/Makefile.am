bin_PROGRAMS = gplatform
gplatform_SOURCES = platform.cxx  i8051sim.cxx i8051system.cxx armsystem.cxx xilinxitf.cxx armsystem.h xilinxitf.h i8051sim.h  i8051system.h  cpicoblaze.cxx cpicoblaze.h cpicoinstr.cxx cpicoinstr.h platform.h picoblaze.cxx picoblaze.h gezelavr.cxx gezelavr.h

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

gplatform_LDADD = -lgmp -larmsim -larmemu -lsim -lbfd -L$(top_builddir)/ipblock -lipconfig -L$(top_builddir)/gezel -lfdl $(LDDYNLIB)
INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock $(SIMITDEF)


 


# bin_PROGRAMS = gplatform
# gplatform_SOURCES = platform.cxx  i8051sim.cxx i8051system.cxx armsystem.cxx xilinxitf.cxx armsystem.h xilinxitf.h i8051sim.h  i8051system.h cpicoblaze.cxx cpicoblaze.h cpicoinstr.cxx cpicoinstr.h platform.h picoblaze.cxx picoblaze.h \
		    atmega128.c atmega_iomanager.c atmega_op.c atmega_parse_config.c atmega_table.c \
		    atmega128.h atmega_decode_tables.h atmega_defs.h atmega_device.h atmega_iomanager.h atmega_opcodes.h atmega_parse_config.h atmega_table.h \
# 		    atmega128sim.cxx atmega128system.cxx atmega128sim.h atmega128system.h 

# if LINKLIBDL
#  LDDYNLIB = -ldl
# else
#  LDDYNLIB =
# endif
#
# if SUPPORTSIMITSFU
#  SIMITDEF = -DSUPPORTSIMITSFU
# else
#   SIMITDEF =
#endif
#
# rundir=@rundir@
# confdir=@confdir@
# conf_DATA=atmel128.xconf mica2.xconf
# appdir=@appdir@
# devicedir=@devicedir@
# EXTRA_DIST=atmega_LICENSE $(conf_DATA)
#
# AM_CFLAGS=`xml2-config --cflags`
#
# LIBS=`xml2-config --libs`
#
# AM_CPPFLAGS=-DRUNDIR=\"@rundir@\" -DCONFDIR=\"@confdir@\" -DAPPDIR=\"@appdir@\" -DDEVICEDIR=\"@devicedir@\"
#AM_CPPFLAGS=-DCONFDIR=\"@confdir@\" @XML_CFLAGS@
#
#gplatform_LDADD = -lgmp -larmsim -larmemu -L$(top_builddir)/ipblock -lipconfig -L$(top_builddir)/gezel -lfdl $(LDDYNLIB) 
#INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock $(SIMITDEF)
# gplatform_LDFLAGS=-rdynamic
