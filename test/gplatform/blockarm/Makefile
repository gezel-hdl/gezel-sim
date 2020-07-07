include ../../Makefile.cfg

TARGET = ramrw

all: $(TARGET)

cycle.o: cycle.s
	$(ARMCC) -static -c -Os cycle.s -o cycle.o

ramrw.o: ramrw.c
	$(ARMCC) -static  -c ramrw.c -o ramrw.o

ramrw: ramrw.o cycle.o
	$(ARMCC) -static  ramrw.o cycle.o -o ramrw


sim:
	$(GPLATFORM) blockio.fdl

clean:
	rm -f *.o $(TARGET) fdl.symbols *~

