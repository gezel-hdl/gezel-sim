include ../../Makefile.cfg

TARGET = cordic_fixp

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) fsl.fdl

vhd:
	$(FDLVHD) -c FSL_Clk FSL_Rst fsl.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~
