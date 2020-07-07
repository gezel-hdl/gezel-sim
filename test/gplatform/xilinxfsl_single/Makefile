include ../../Makefile.cfg

TARGET = fsldrive

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM)  fsl2.fdl

vhd:
	$(FDLVHD) -c FSL_Clk FSL_Rst fsl2.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~ *.vhd
