all: sim

sim: small.dec
	/opt/gezel/bin/gplatform -c 500 pb.fdl

small.dec: small.psm
	/opt/KCPSM3/dosemu/picoasm small.psm

clean:
	rm -f gmon.out labels.txt small.coe small.fmt small.log small.mem small.v *.vhd small.m small.dec small.hex pass*.dat *~ constant.txt
	rm -rf vhd/*
	rm -f unisim/*.o unisim/*.cf





