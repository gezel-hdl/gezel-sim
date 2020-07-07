include ../../Makefile.cfg

TARGET = cycle

all: $(TARGET)

%: %.c
	$(ARMCC) -static -O3 $< -o $@

sim:
	$(GPLATFORM) cycle.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
