include ../../Makefile.cfg

TARGET = cordic_fixp

all: $(TARGET)

%: %.c
	$(ARMCC) -static -O3 $< -o $@

sim: sim_fixp

sim_fixp:
	$(GPLATFORM) fsl.fdl

vhd:
	$(FDLVHD) -c FSL_Clk FSL_Rst fsl.fdl

clean:
	rm -f *.o $(TARGET) fdl.symbols *~ *.vhd
