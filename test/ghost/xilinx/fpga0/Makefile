.PHONY: clean

ENTITY = fpga0

UCF = ./fpga0.ucf

PART = XC5VSX240T-ff1738-1

SRC    = $(wildcard ./src/*.vhd)

CORE_DIR = /home/schaum/Devel/nallatech_edge_cores/
CORE_DIR1 = ./coregen/


$(ENTITY).bit: $(ENTITY).ut $(ENTITY).ncd $(ENTITY).pcf $(ENTITY).twr
	bitgen -d -intstyle silent -f $(ENTITY).ut $(ENTITY).ncd $(ENTITY).bit $(ENTITY).pcf

$(ENTITY).twr: $(ENTITY).ncd $(ENTITY).pcf
	trce -intstyle xflow -e 300 -fastpaths -xml $(ENTITY).twx $(ENTITY).ncd -o $(ENTITY).twr $(ENTITY).pcf

$(ENTITY).ncd: $(ENTITY)_map.ncd $(ENTITY).pcf
	par -intstyle xflow -ol high -xe n -w $(ENTITY)_map.ncd $(ENTITY).ncd $(ENTITY).pcf

$(ENTITY)_map.ncd: $(ENTITY).ngd
	map -detail -intstyle xflow -timing -cm speed -pr b -xe n -p $(PART) -w -o $(ENTITY)_map.ncd $(ENTITY).ngd $(ENTITY).pcf

$(ENTITY).ngd: $(ENTITY).ngc $(UCF) 
	ngdbuild -intstyle xflow -dd ngo -nt timestamp -sd $(CORE_DIR) -sd $(CORE_DIR1) -uc $(UCF)  -p $(PART) $(ENTITY).ngc $(ENTITY).ngd

$(ENTITY).ngc: $(SRC) $(ENTITY).xst $(ENTITY).prj $(ENTITY).lso
	xst -intstyle xflow -ifn $(ENTITY).xst -ofn $(ENTITY).syr

clean :
	rm -f $(ENTITY).ngc $(ENTITY).ngr $(ENTITY).syr 
	rm -rf ./xst
	rm -f $(ENTITY).bld $(ENTITY).ngd
	rm -rf ./ngo
	rm -f acp_top_fc_map.* $(ENTITY).pcf
	rm -f $(ENTITY).ncd $(ENTITY).par $(ENTITY).pad $(ENTITY)_pad.* $(ENTITY).unroutes $(ENTITY).xpi
	rm -f *.syr *.bld *.twr $(ENTITY).twx
	rm -f $(ENTITY).bit $(ENTITY).bgn $(ENTITY).drc *.xml
	rm -f $(ENTITY).bin
	rm -f $(ENTITY)_map*
	rm -f $(ENTITY)_ngdbuild*
	rm -f $(ENTITY)_par*
	rm -f $(ENTITY)_xst*
	rm -rf xlnx_auto_0_xdb
	rm -f $(ENTITY).ptwx
	rm -f *~

       
cleanexceptbit :
	rm -f $(ENTITY).ngc $(ENTITY).ngr $(ENTITY).syr
	rm -rf ./xst
	rm -f $(ENTITY).bld $(ENTITY).ngd
	rm -rf ./ngo
	rm -f acp_top_fc_map.* $(ENTITY).pcf
	rm -f $(ENTITY).ncd $(ENTITY).par $(ENTITY).pad $(ENTITY)_pad.* $(ENTITY).unroutes $(ENTITY).xpi
	rm -f *.twr $(ENTITY).twx	rm -f $(ENTITY).bgn $(ENTITY).drc *.xml
