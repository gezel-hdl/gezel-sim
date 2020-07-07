if BUILDSTANDALONE
  STANDALONEDIR = standalone
else
  STANDALONEDIR = 
endif

if BUILDSYSTEMCCOSIM
  SYSTEMCCOSIMDIR = systemc
else
  SYSTEMCCOSIMDIR = 
endif

if BUILDJAVA
  JAVADIR = java
else
  JAVADIR =
endif

if BUILDGPLATFORM
  GPLATFORMDIR = gplatform
else
  GPLATFORMDIR =
endif

SUBDIRS = m4 gezel ipblock $(SYSTEMCCOSIMDIR) $(JAVADIR) $(GPLATFORMDIR) $(STANDALONEDIR)

# find test -name '*' -type f | grep -v '.svn'

EXTRA_DIST = config.rpath  gezel/fdl.y                        \
        BUGS                                                  \
	gezel/fdl.ll                                          \
	java/GezelInport.java                                 \
	java/GezelModule.java                                 \
	java/GezelOutport.java                                \
	test/systemc/accum_sc.cxx \
	test/systemc/Makefile \
	test/systemc/accum.fdl \
	test/gplatform/xilinxfsl_single/fsl2.fdl \
	test/gplatform/xilinxfsl_single/Makefile \
	test/gplatform/xilinxfsl_single/PLB_EDK92 \
	test/gplatform/xilinxfsl_single/fsldrive.c \
	test/gplatform/xilinxipiffifo/fifodrive.c \
	test/gplatform/xilinxipiffifo/OPB_EDK8 \
	test/gplatform/xilinxipiffifo/user.fdl \
	test/gplatform/xilinxipiffifo/Makefile \
	test/gplatform/ssidehsk/receiver.c \
	test/gplatform/ssidehsk/sender.c \
	test/gplatform/ssidehsk/Makefile \
	test/gplatform/ssidehsk/network.fdl \
	test/gplatform/fsl2/fsl.fdl \
	test/gplatform/fsl2/Makefile \
	test/gplatform/fsl2/fsldrive.c \
	test/gplatform/hello51/driver.c \
	test/gplatform/hello51/hello.fdl \
	test/gplatform/hello51/Makefile \
	test/gplatform/gfmul/slave51.c \
	test/gplatform/gfmul/drivearm.c \
	test/gplatform/gfmul/Makefile \
	test/gplatform/gfmul/mpsoc.fdl \
	test/gplatform/blockarm/ramrw.c \
	test/gplatform/blockarm/cycle.s \
	test/gplatform/blockarm/Makefile \
	test/gplatform/blockarm/blockio.fdl \
	test/gplatform/xilinxipifreg_sub64/ipreg_example.fdl \
	test/gplatform/xilinxipifreg_sub64/ipreg_drive.c \
	test/gplatform/xilinxipifreg_sub64/Makefile \
	test/gplatform/xilinxipifreg_sub64/PLB_EDK92 \
	test/gplatform/euclid/euclid.c \
	test/gplatform/euclid/euclid.fdl \
	test/gplatform/euclid/Makefile \
	test/gplatform/ramprobe/Makefile \
	test/gplatform/ramprobe/ramprobe.c \
	test/gplatform/ramprobe/ramprobe.fdl \
	test/gplatform/xilinxipifmem_gcd/OPB_EDK8 \
	test/gplatform/xilinxipifmem_gcd/ipmem_drive.c \
	test/gplatform/xilinxipifmem_gcd/Makefile \
	test/gplatform/xilinxipifmem_gcd/ipmem_example.fdl \
	test/gplatform/armsystemc/armsystem.h \
	test/gplatform/armsystemc/armsystem.cxx \
	test/gplatform/armsystemc/syscarmdriver.c \
	test/gplatform/armsystemc/accum_sc.cxx \
	test/gplatform/armsystemc/Makefile \
	test/gplatform/armsystemc/accum.fdl \
	test/gplatform/xilinxipifreg_gcd/ipreg_example.fdl \
	test/gplatform/xilinxipifreg_gcd/edk92_gcdmm.zip \
	test/gplatform/xilinxipifreg_gcd/ipreg_drive.c \
	test/gplatform/xilinxipifreg_gcd/Makefile \
	test/gplatform/xilinxipifreg_gcd/PLB_EDK92 \
	test/gplatform/block8051/ramrw.c \
	test/gplatform/block8051/Makefile \
	test/gplatform/block8051/blockio.fdl \
	test/gplatform/xilinxfsl_cordic/cordic_fixp.c \
	test/gplatform/xilinxfsl_cordic/fsl.fdl \
	test/gplatform/xilinxfsl_cordic/Makefile \
	test/gplatform/xilinxfsl_cordic/PLB_EDK92 \
	test/gplatform/xilinxfsl_cordic/edk92_cordicfsl.zip \
	test/gplatform/diblaze/Makefile \
	test/gplatform/diblaze/pb.fdl \
	test/gplatform/diblaze/receiver.psm \
	test/gplatform/diblaze/sender.psm \
	test/gplatform/Makefile \
	test/gplatform/helloarm/hellomodel.fdl \
	test/gplatform/helloarm/hello.c \
	test/gplatform/helloarm/Makefile \
	test/gplatform/arm2arm/Makefile \
	test/gplatform/arm2arm/writer.c \
	test/gplatform/arm2arm/reader.c \
	test/gplatform/arm2arm/intf.fdl \
	test/gplatform/aes8051/driver.c \
	test/gplatform/aes8051/Makefile \
	test/gplatform/aes8051/aes.fdl \
	test/gplatform/armsfu/sfudriver.c \
	test/gplatform/armsfu/armsfu.fdl \
	test/gplatform/armsfu/Makefile \
	test/gplatform/armsfu/arm_ext.h \
	test/gplatform/aesarm/aes_coproc_armdriver.c \
	test/gplatform/aesarm/aes_coproc_arm.fdl \
	test/gplatform/aesarm/Makefile \
	test/gplatform/cordicfsl/fsl_ref.fdl \
	test/gplatform/cordicfsl/cordic_ref.c \
	test/gplatform/cordicfsl/cordic_fixp.c \
	test/gplatform/cordicfsl/fsl.fdl \
	test/gplatform/cordicfsl/cycle.s \
	test/gplatform/cordicfsl/Makefile \
	test/gplatform/pblaze/small.psm \
	test/gplatform/pblaze/Makefile \
	test/gplatform/pblaze/pb.fdl \
	test/gplatform/xilinxipiffifo_gcd/fifodrive.c \
	test/gplatform/xilinxipiffifo_gcd/OPB_EDK8 \
	test/gplatform/xilinxipiffifo_gcd/user.fdl \
	test/gplatform/xilinxipiffifo_gcd/Makefile \
	test/gplatform/cyclecount/cycle.c \
	test/gplatform/cyclecount/cycle.fdl \
	test/gplatform/cyclecount/Makefile \
	test/gplatform/fslinterface/fsl.fdl \
	test/gplatform/fslinterface/Makefile \
	test/gplatform/fslinterface/fsldrive.c \
	test/ipblock/Makefile \
	test/ipblock/lutram/Makefile \
	test/ipblock/lutram/lutram.cxx \
	test/ipblock/lutram/lutram.h \
	test/ipblock/lutram/ramcell.fdl \
	test/gezel/ramblock2.fdl \
	test/gezel/euclid.fdl \
	test/gezel/gf8inv.fdl \
	test/gezel/toggle.fdl \
	test/gezel/Makefile \
	test/gezel/aes.fdl \
	test/gezel/scripted.fdl \
	test/gezel/ipchecksum.fdl \
	test/gezel/lfsr.fdl \
	test/gezel/ramblock.fdl \
	test/gezel/bresen.fdl \
	test/java/euclid/euclid.fdl \
	test/java/euclid/Makefile \
	test/java/euclid/euclid.java \
	test/java/avrora2/simple.c \
	test/java/avrora2/hwsw.java \
	test/java/avrora2/Makefile \
	test/java/avrora2/module.fdl \
	test/java/counter2/Makefile \
	test/java/counter2/counter2.java \
	test/java/counter2/counter2.fdl \
	test/java/counter3/counter3.fdl \
	test/java/counter3/counter3.java \
	test/java/counter3/Makefile \
	test/java/avrora1/simple.c \
	test/java/avrora1/hwsw.java \
	test/java/avrora1/Makefile \
	test/java/avrora1/module.fdl \
	test/java/Makefile \
	test/java/counter1/counter1.fdl \
	test/java/counter1/counter1.java \
	test/java/counter1/Makefile \
	test/Makefile \
	test/Makefile.cfg

# \
#	po/Makefile.in.in

ACLOCAL_AMFLAGS = -I m4
