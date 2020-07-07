# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $

include ../../Makefile.cfg

euclid.class: euclid.java
	$(JAVAC) -classpath ../../../build/share euclid.java

sim: euclid.class
	export LD_LIBRARY_PATH=../../../build/lib; $(JAVA) -classpath ../../../build/share:. euclid

clean:
	rm -f *.class
