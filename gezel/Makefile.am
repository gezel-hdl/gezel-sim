fdllex.cc: fdllex.ll
	$(LEX) $(LFLAGS) $(AM_LFLAGS) -Pfdl -o"$@" "$<"

fdlyacc.cc fdlyacc.h: fdlyacc.yy
	$(YACC) -d -pfdl -ofdlyacc.cc fdlyacc.yy
	@mv fdlyacc.cc.h fdlyacc.h 2>/dev/null || mv fdlyacc.hh fdlyacc.h || true

BUILT_SOURCES = fdlyacc.h
CLEANFILES = fdllex.cc fdlyacc.cc fdlyacc.h

lib_LTLIBRARIES = libfdl.la

libfdl_la_SOURCES =     \
          fdllex.ll     \
          fdlyacc.yy    \
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

libfdl_la_LIBADD = -lgmp -lstdc++

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

.PHONY: dist-hook
dist-hook:
	cd "$(distdir)" && rm $(CLEANFILES)
