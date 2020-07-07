include ../../Makefile.cfg

driver.ihx: driver.c simcmd
	$(SDCC) driver.c

simcmd:
	@echo "To simulate, type 'make sim'"

sim: driver.ihx
	$(GPLATFORM) aes.fdl

clean:
	rm -f *~ *.asm *.lnk *.map *.rel *.sym *.rst *.mem *.ihx *.lst
