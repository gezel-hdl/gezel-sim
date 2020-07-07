include ../../Makefile.cfg

TARGET = sfudriver

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) armsfu.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
