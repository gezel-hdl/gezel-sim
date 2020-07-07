include ../../Makefile.cfg

TARGET = fsldrive

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) fsl.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
