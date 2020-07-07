# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $

include ../../Makefile.cfg

counter3.class: counter3.java
	$(JAVAC) -classpath ../../../build/share counter3.java

sim: counter3.class
	export LD_LIBRARY_PATH=../../../build/lib; $(JAVA) -classpath ../../../build/share:. counter3

clean:
	rm -f *.class
