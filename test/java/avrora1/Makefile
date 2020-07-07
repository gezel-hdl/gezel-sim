# $Id: Makefile,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $

include ../../Makefile.cfg

GEZELCLS=../../../build/share
GEZELLIB=../../../build/lib

TARGET = srec
TARGETS = $(TARGET).srec

#-- platform

hwsw.class: hwsw.java
	 $(JAVAC) -classpath $(AVRORACLS):$(GEZELCLS) hwsw.java

sim: app.od module.fdl
	export LD_LIBRARY_PATH=$(GEZELLIB); $(JAVA) -classpath $(AVRORACLS):.:$(GEZELCLS) avrora.Main -monitors=energyProfiler -colors=false -platform=hwsw app.od

#-- application
app.od: simple.c
	avr-gcc -mmcu=atmega128 -ggdb simple.c -o app.out
	avr-objdump -zhD app.out >app.od

clean:
	rm -f *.class javacore.* *~ *.od *.out

