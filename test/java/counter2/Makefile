# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:46 schaum Exp $

include ../../Makefile.cfg

counter2.class: counter2.java
	$(JAVAC) -classpath ../../../build/share counter2.java

sim: counter2.class
	export LD_LIBRARY_PATH=../../../build/lib; $(JAVA) -classpath ../../../build/share:. counter2

clean:
	rm -f *.class
