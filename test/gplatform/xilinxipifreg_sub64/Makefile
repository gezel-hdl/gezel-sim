include ../../Makefile.cfg

TARGET = ipreg_drive

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM)  ipreg_example.fdl

vhd:
	$(FDLVHD) -c Bus2IP_Clk Bus2IP_Rst ipreg_example.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~ *.vhd
