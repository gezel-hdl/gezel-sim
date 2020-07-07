SMALL.HEX: small.psm
	wine /opt/KCPSM3/Assembler/KCPSM3.EXE small.psm >& /dev/null

sim: SMALL.HEX
	$(GPLATFORM) -c 5000 pb.fdl

clean:
	rm -f SMALL.COE  SMALL.FMT  SMALL.LOG  SMALL.MEM  SMALL.VHD  SMALL.HEX  SMALL.M    SMALL.V  *~ PASS*.DAT LABELS.TXT CONSTANT.TXT


