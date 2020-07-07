include ../../Makefile.cfg

TARGET = sender receiver

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) network.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
