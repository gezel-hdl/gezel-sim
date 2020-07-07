include ../../Makefile.cfg

TARGET = euclid

all: $(TARGET)

%: %.c
	$(ARMCC) -static -O3 $< -o $@

sim:
	$(GPLATFORM) euclid.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
