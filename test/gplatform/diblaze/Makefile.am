include ../../Makefile.cfg
include ../../picoblaze.cfg

CYCLES ?= 1000

TARGET = sender.dec receiver.dec

.PHONY: all
all: $(TARGET)

.PHONY: sim
sim: $(PB_OBJ) $(RUN)
	$(GPLATFORM) -c $(CYCLES) pb.fdl

.PHONY: clean-local
clean-local:
	rm -f *~ pass*.dat labels.txt constant.txt *.dec *.fmt *.hex *.log *.m *.mem \
		$(SENDER).coe $(SENDER).v $(SENDER).vhd \
		$(RECEIVER).coe $(RECEIVER).v $(RECEIVER).vhd

EXTRA_DIST =        \
    pb.fdl          \
    receiver.psm    \
    sender.psm
