SUBDIRS =     \
    gezel     \
    gplatform \
    ipblock   \
    java

.PHONY: sim
sim:
	make -C gezel sim
#	make -C gplatform sim
	make -C ipblock sim
	make -C java sim

.PHONY: xtra
xtra:
	make -C systemc sim

EXTRA_DIST =        \
	Makefile.cfg    \
	arm.cfg         \
	avr.cfg         \
	picoblaze.cfg   \
	sdcc.cfg
