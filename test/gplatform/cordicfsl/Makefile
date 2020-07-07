include ../../Makefile.cfg

TARGET = cordic_fixp cordic_ref

all: $(TARGET)

%: %.c
	$(ARMCC) -static -O3 cycle.s $< -o $@

sim: sim_fixp sim_ref

sim_fixp:
	$(GPLATFORM) fsl.fdl

sim_ref:
	$(GPLATFORM) fsl_ref.fdl

vhd:
	$(FDLVHD) -c FSL_Clk FSL_Rst fsl.fdl

clean:
	rm -f *.o $(TARGET) fdl.symbols *~
