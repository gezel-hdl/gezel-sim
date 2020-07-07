include ../../Makefile.cfg

TARGET = fifodrive

all: $(TARGET)

%: %.c
	$(ARMCC) -static  $< -o $@

sim:
	../../../build/bin/gplatform user.fdl

vhd:
	../../../../cgen/build/bin/fdlvhd -c Bus2IP_Clk Bus2IP_Reset user.fdl

clean:
	rm -f *.o $(TARGET) *.s fdl.symbols *~ *.vhd
