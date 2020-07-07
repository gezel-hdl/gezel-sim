GezelModule.h GezelModule.class: GezelModule.java
	$(JAVAC) GezelModule.java
	$(JAVAH) GezelModule

GezelInport.h GezelInport.class: GezelInport.java
	$(JAVAC) GezelInport.java
	$(JAVAH) GezelInport

GezelOutport.h GezelOutport.class: GezelOutport.java
	$(JAVAC) GezelOutport.java
	$(JAVAH) GezelOutport

clean-local:
	rm -f GezelModule.h GezelInport.h GezelOutport.h *.class

GezelModuleImp.cxx: GezelModule.h

GezelInportImp.cxx: GezelInport.h

GezelOutportImp.cxx: GezelOutport.h

lib_LTLIBRARIES = libgzljava.la

BUILT_SOURCES =         \
    GezelModule.h       \
    GezelInport.h       \
    GezelOutport.h

libgzljava_la_SOURCES = \
    GezelModuleImp.cxx  \
    GezelInportImp.cxx  \
    GezelOutportImp.cxx \
    java_itf.h

libgzljava_la_LIBADD = ../ipblock/libipconfig.la ../gezel/libfdl.la

AM_CPPFLAGS = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock

data_DATA =             \
    GezelModule.class   \
    GezelInport.class   \
    GezelOutport.class

EXTRA_DIST = \
	GezelInport.java \
	GezelModule.java \
	GezelOutport.java
