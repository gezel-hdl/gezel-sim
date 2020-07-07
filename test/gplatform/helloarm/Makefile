include ../../Makefile.cfg

TARGET = hello

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) hellomodel.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
