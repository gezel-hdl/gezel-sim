lib_LTLIBRARIES = libnallasim.la

AM_CPPFLAGS = -I$(top_srcdir)/gezel  -I$(top_srcdir)/ipblock

libnallasim_la_SOURCES =    \
    nallasim.cxx

libnallasim_la_LIBADD = ../ipblock/libipconfig.la ../gezel/libfdl.la

pkginclude_HEADERS = nallasim.h

