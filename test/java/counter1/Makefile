# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $

include ../../Makefile.cfg

counter1.class: counter1.java
	$(JAVAC) -classpath ../../../java counter1.java

sim: counter1.class
	export LD_LIBRARY_PATH=../../../build/lib; $(JAVA) -classpath ../../../build/share:. counter1

clean:
	rm -f *.class
