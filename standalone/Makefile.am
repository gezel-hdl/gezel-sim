bin_PROGRAMS = fdlsim 
fdlsim_SOURCES = fdlsim.cxx

if LINKLIBDL
  LDDYNLIB = -ldl
else
  LDDYNLIB =
endif

# use for statically linked executable
fdlsim_LDFLAGS = -static

fdlsim_LDADD = -lgmp -L$(top_builddir)/ipblock -lipconfig -L$(top_builddir)/gezel -lfdl $(LDDYNLIB)

INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock 
