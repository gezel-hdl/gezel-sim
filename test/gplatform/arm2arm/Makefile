include ../../Makefile.cfg

TARGET = writer reader

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) intf.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
