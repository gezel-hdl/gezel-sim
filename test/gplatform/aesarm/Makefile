include ../../Makefile.cfg

TARGET = aes_coproc_armdriver

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM)  aes_coproc_arm.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
