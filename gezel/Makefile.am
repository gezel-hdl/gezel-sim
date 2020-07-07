
fdlyacc.cc fdlyacc.h: fdl.y
	bison -d -pfdl -ofdlyacc.cc fdl.y
	@mv fdlyacc.cc.h fdlyacc.h 2>/dev/null || mv fdlyacc.hh fdlyacc.h

BUILT_SOURCES = fdlyacc.cc fdl.cc fdlyacc.h

AM_LFLAGS=-Pfdl -olex.yy.c

lib_LTLIBRARIES = libfdl.la

CORELIB = fdl.ll        \
	  fdlyacc.cc    \
          artsignal.cxx \
          asymbol.cxx   \
          fdlcode.cxx   \
          fdlsym.cxx    \
          gval.cxx      \
          ipblock.cxx   \
          module.cxx    \
          rtbinop.cxx   \
          rtctl.cxx     \
          rtdp.cxx      \
          rterror.cxx   \
          rtio.cxx      \
          rtlookup.cxx  \
          rtopt.cxx     \
          rtprofile.cxx \
          rtsignal.cxx  \
          rtsim.cxx     \
          rtsleep.cxx   \
          rttrace.cxx   \
          rtunop.cxx

libfdl_la_SOURCES = $(CORELIB)

pkginclude_HEADERS =    \
          artsignal.h   \
          asymbol.h     \
          fdlcode.h     \
          fdlparse.h    \
          fdlsym.h      \
          fdlyacc.h     \
          gval.h        \
          ipblock.h     \
          module.h      \
          rtbinop.h     \
          rtctl.h       \
          rtdp.h        \
          rterror.h     \
          rtio.h        \
          rtlookup.h    \
          rtopt.h       \
          rtprofile.h   \
          rtsignal.h    \
          rtsim.h       \
          rtsleep.h     \
          rttrace.h     \
          rtunop.h



