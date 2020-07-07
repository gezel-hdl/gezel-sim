include ../../Makefile.cfg

TARGET = avr_check

MCU_TARGET     = atmega128
OPTIMIZE       = -Os
DEFS           =
LIBS           =
AVRGCC             = avr-gcc
AVROBJCOPY        = avr-objcopy
AVROBJDUMP        = avr-objdump

override CFLAGS        = -g -mmcu=$(MCU_TARGET) $(DEFS)

all: $(TARGET).elf lst srec

%.o: %.c
	$(AVRGCC) $(OPTIMIZE) $(CFLAGS) -c $<

%.elf: %.o
	$(AVRGCC) $(CFLAGS) -o $@ $^ $(LIBS)

lst: $(TARGET).lst

%.lst: %.elf
	$(AVROBJDUMP) -h -S $< > $@

srec: $(TARGET).srec

%.srec: %.elf
	$(AVROBJCOPY) -j .text -j .data -O srec $< $@


sim: sim_fixp

sim_fixp:
	$(GPLATFORM) avr_check.fdl

clean:
	rm -f *.o $(TARGET) fdl.symbols *~ *.vhd $(TARGET).elf *.bak *.srec
	rm -f *.lst *.map $(EXTRA_CLEAN_FILES)
