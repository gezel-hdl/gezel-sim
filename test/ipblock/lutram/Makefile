include ../../Makefile.cfg

all: liblutram.so

sim:
	$(FDLSIM) ramcell.fdl 32

lutram.o: lutram.cxx
	g++ -fPIC  -Wall -c -I$(FDLINC) lutram.cxx

liblutram.so: lutram.o
	g++ -shared lutram.o  			\
		-Wl,--rpath -Wl,$(FDLLIB)       \
		$(FDLLIB)/libipconfig.so 	\
		$(FDLLIB)/libfdl.so 		\
		-lgmp -ldl -o liblutram.so

clean:
	rm -f *.o *.so *~ *.vhd
