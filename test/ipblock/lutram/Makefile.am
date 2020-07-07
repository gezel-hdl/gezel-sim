include ../../Makefile.cfg

lib_LTLIBRARIES = liblutram.la

liblutram_la_SOURCES = \
    lutram.cxx

AM_CPPFLAGS = -I$(top_srcdir)/ipblock -I$(top_srcdir)/gezel
liblutram_la_LIBADD = $(top_builddir)/ipblock/libipconfig.la $(top_builddir)/gezel/libfdl.la
liblutram_la_LDFLAGS = -module

sim:
	cd .libs && $(FDLSIM) $(abs_srcdir)/ramcell.fdl 32

EXTRA_DIST =    \
    ramcell.fdl
