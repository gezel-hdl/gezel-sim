include ../../Makefile.cfg

RUN=pb.fdl
CYCLES=1000
RECEIVER=receiver
SENDER=sender
PB_OBJ=$(SENDER).dec $(RECEIVER).dec

%.dec: %.psm
	$(PICOASM) $<

all: $(PB_OBJ)

sim: $(PB_OBJ) $(RUN)
	$(GPLATFORM) -c $(CYCLES) $(RUN)

clean:
	rm -f *~ pass*.dat labels.txt constant.txt *.dec *.fmt *.hex *.log *.m *.mem \
$(SENDER).coe $(SENDER).v $(SENDER).vhd \
$(RECEIVER).coe $(RECEIVER).v $(RECEIVER).vhd

