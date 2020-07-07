INC=../../../build/include/gezel
LIB=../../../build/lib

userapp: userapp.cpp
	g++ -g userapp.cpp -I$(INC) -L$(LIB) -lnallasim -lipconfig -lfdl -ldl -Wl,-rpath -Wl,$(LIB) -o userapp

clean:
	rm -f *~ userapp *.vhd fdl.symbols fpga0.ucf



 
