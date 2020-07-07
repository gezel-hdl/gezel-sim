include ../../Makefile.cfg

slave51.ihx: slave51.c
	$(SDCC) slave51.c

drivearm: drivearm.c
	$(ARMCC) -static drivearm.c -o drivearm

sim: slave51.ihx drivearm
	$(GPLATFORM) mpsoc.fdl

clean:
	rm -f *~ *.asm *.lnk *.map *.rel *.sym *.rst *.mem *.ihx *.lst drivearm
