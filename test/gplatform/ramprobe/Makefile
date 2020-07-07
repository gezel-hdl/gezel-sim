include ../../Makefile.cfg

ramprobe: ramprobe.c
	$(ARMCC) -static ramprobe.c -o ramprobe

sim:
	$(GPLATFORM) ramprobe.fdl

clean:
	rm -f *.o *~ ramprobe
