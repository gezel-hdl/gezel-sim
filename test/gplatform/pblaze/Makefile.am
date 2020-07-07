include ../../Makefile.cfg
include ../../picoblaze.cfg

TARGET = small.dec

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(TARGET)
	/opt/gezel/bin/gplatform -c 500 pb.fdl

.PHONY: clean-local
clean-local:
	rm -f gmon.out labels.txt small.coe small.fmt small.log small.mem small.v *.vhd small.m small.dec small.hex pass*.dat *~ constant.txt
	rm -rf vhd/*
	rm -f unisim/*.o unisim/*.cf

EXTRA_DIST =    \
    pb.fdl      \
    small.psm
