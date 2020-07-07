include ../../Makefile.cfg

ramrw.ihx: ramrw.c simcmd
	$(SDCC) --model-large ramrw.c

simcmd:
	@echo "To simulate, type 'make sim'"  

sim: ramrw.ihx
	$(GPLATFORM) -c 100000 blockio.fdl

clean:
	rm -f *~ *.asm *.lnk *.map *.rel *.sym *.rst *.mem *.ihx *.lst
