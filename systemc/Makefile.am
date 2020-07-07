lib_LTLIBRARIES = libgzlsysc.la

libgzlsysc_la_SOURCES =  systemc_itf.h systemc_itf.cxx 

libgzlsysc_la_LIBADD = -lgmp -L$(top_srcdir)/ipblock -lipconfig -L$(top_srcdir)/gezel -lfdl -lstdc++

INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock 

pkginclude_HEADERS = systemc_itf.h
