JAVAC=/opt/IBMJava2-142/bin/javac
JAVAH=/opt/IBMJava2-142/bin/javah

GezelModule.h GezelModule.class: GezelModule.java
	$(JAVAC) GezelModule.java
	$(JAVAH) GezelModule

GezelInport.h GezelInport.class: GezelInport.java
	$(JAVAC) GezelInport.java
	$(JAVAH) GezelInport

GezelOutport.h GezelOutport.class: GezelOutport.java
	$(JAVAC) GezelOutport.java
	$(JAVAH) GezelOutport

clean:
	rm -f GezelModule.h GezelInport.h GezelOutport.h *.class

GezelModuleImp.cxx: GezelModule.h

GezelInportImp.cxx: GezelInport.h

GezelOutportImp.cxx: GezelOutport.h

lib_LTLIBRARIES = libgzljava.la

libgzljava_la_SOURCES = GezelModule.h GezelInport.h GezelOutport.h GezelModuleImp.cxx GezelInportImp.cxx GezelOutportImp.cxx java_itf.h

libgzljava_la_LIBADD = -lgmp -L$(top_srcdir)/ipblock -lipconfig -L$(top_srcdir)/gezel -lfdl -lstdc++

INCLUDES = -I$(top_srcdir)/gezel -I$(top_srcdir)/ipblock 

data_DATA = GezelModule.class GezelInport.class GezelOutport.class
