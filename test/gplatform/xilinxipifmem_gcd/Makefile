include ../../Makefile.cfg

TARGET = ipmem_drive

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	$(GPLATFORM) -c 50000 ipmem_example.fdl

vhd:
	$(FDLVHD) -c Bus2IP_Clk Bus2IP_Reset ipmem_example.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~ *.vhd
