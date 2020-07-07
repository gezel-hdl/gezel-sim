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
	test/Makefile					      \
	test/gplatform/fslinterface/Makefile		      \
	test/gplatform/fslinterface/fsldrive.c		      \
	test/gplatform/fslinterface/fsl.fdl		      \
	test/gplatform/cordicfsl/cordic_fixp.c		      \
	test/gplatform/cordicfsl/cordic_ref.c		      \
	test/gplatform/cordicfsl/fsl.fdl		      \
	test/gplatform/cordicfsl/Makefile		      \
	test/gplatform/cordicfsl/Makefile.cfg		      \
	test/gplatform/euclid/Makefile			      \
	test/gplatform/euclid/euclid.fdl		      \
	test/gplatform/euclid/euclid.c			      \
	test/gplatform/Makefile				      \
	test/gplatform/xilinxfsl_split/Makefile		      \
	test/gplatform/xilinxfsl_split/fsldrive.c	      \
	test/gplatform/xilinxfsl_split/fsl.fdl		      \
	test/gplatform/armsystemc/accum.fdl		      \
	test/gplatform/armsystemc/Makefile		      \
	test/gplatform/armsystemc/accum_sc.cxx		      \
	test/gplatform/armsystemc/syscarmdriver.c	      \
	test/gplatform/armsystemc/armsystem.h		      \
	test/gplatform/armsystemc/armsystem.cxx		      \
	test/gplatform/fsl2/Makefile			      \
	test/gplatform/fsl2/fsldrive.c			      \
	test/gplatform/fsl2/fsl.fdl			      \
	test/gplatform/xilinxipifmem_gcd/Makefile	      \
	test/gplatform/xilinxipifmem_gcd/ipmem_example.fdl    \
	test/gplatform/xilinxipifmem_gcd/ipmem_drive.c	      \
	test/gplatform/xilinxipifreg_gcd/Makefile	      \
	test/gplatform/xilinxipifreg_gcd/ipreg_drive.c	      \
	test/gplatform/xilinxipifreg_gcd/ipreg_example.fdl    \
	test/gplatform/blockarm/Makefile		      \
	test/gplatform/blockarm/cycle.s			      \
	test/gplatform/blockarm/blockio.fdl		      \
	test/gplatform/blockarm/ramrw.c			      \
	test/gplatform/armsfu/arm_ext.h			      \
	test/gplatform/armsfu/Makefile			      \
	test/gplatform/armsfu/sfudriver.c		      \
	test/gplatform/armsfu/armsfu.fdl		      \
	test/gplatform/gfmul/Makefile			      \
	test/gplatform/gfmul/mpsoc.fdl			      \
	test/gplatform/gfmul/drivearm.c			      \
	test/gplatform/gfmul/slave51.c			      \
	test/gplatform/xilinxfsl_single/Makefile	      \
	test/gplatform/xilinxfsl_single/fsl2.fdl	      \
	test/gplatform/xilinxfsl_single/fsldrive.c	      \
	test/gplatform/xilinxipiffifo/user.fdl		      \
	test/gplatform/xilinxipiffifo/makefile		      \
	test/gplatform/xilinxipiffifo/fifodrive.c	      \
	test/gplatform/diblaze/RECEIVER.psm		      \
	test/gplatform/diblaze/pb.fdl			      \
	test/gplatform/diblaze/Makefile			      \
	test/gplatform/diblaze/ROM_form.coe		      \
	test/gplatform/diblaze/ROM_form.v		      \
	test/gplatform/diblaze/SENDER.psm		      \
	test/gplatform/diblaze/ROM_form.vhd		      \
	test/gplatform/aesarm/Makefile			      \
	test/gplatform/aesarm/aes_coproc_armdriver.c	      \
	test/gplatform/aesarm/aes_coproc_arm.fdl	      \
	test/gplatform/aes8051/driver.c			      \
	test/gplatform/aes8051/Makefile			      \
	test/gplatform/aes8051/aes.fdl			      \
	test/gplatform/arm2arm/intf.fdl			      \
	test/gplatform/arm2arm/Makefile			      \
	test/gplatform/arm2arm/writer.c			      \
	test/gplatform/arm2arm/reader.c			      \
	test/gplatform/block8051/Makefile		      \
	test/gplatform/block8051/blockio.fdl		      \
	test/gplatform/block8051/ramrw.c		      \
	test/gplatform/cyclecount/Makefile		      \
	test/gplatform/cyclecount/cycle.c		      \
	test/gplatform/cyclecount/cycle.fdl		      \
	test/gplatform/xilinxipifreg_sub64/Makefile	      \
	test/gplatform/xilinxipifreg_sub64/ipreg_drive.c      \
	test/gplatform/xilinxipifreg_sub64/ipreg_example.fdl  \
	test/gplatform/pblaze/pb.fdl			      \
	test/gplatform/pblaze/Makefile			      \
	test/gplatform/pblaze/ROM_form.coe		      \
	test/gplatform/pblaze/ROM_form.v		      \
	test/gplatform/pblaze/SMALL.DEC			      \
	test/gplatform/pblaze/small.psm			      \
	test/gplatform/pblaze/ROM_form.vhd		      \
	test/gplatform/xilinxipiffifo_gcd/user.fdl	      \
	test/gplatform/xilinxipiffifo_gcd/makefile	      \
	test/gplatform/xilinxipiffifo_gcd/fifodrive.c	      \
	test/gplatform/helloarm/Makefile		      \
	test/gplatform/helloarm/hellomodel.fdl		      \
	test/gplatform/helloarm/hello.c			      \
	test/gplatform/ssidehsk/network.fdl		      \
	test/gplatform/ssidehsk/Makefile		      \
	test/gplatform/ssidehsk/sender.c		      \
	test/gplatform/ssidehsk/receiver.c		      \
	test/gplatform/ramprobe/Makefile		      \
	test/gplatform/ramprobe/ramprobe.c		      \
	test/gplatform/ramprobe/ramprobe.fdl		      \
	test/gplatform/hello51/hello.fdl		      \
	test/gplatform/hello51/driver.c			      \
	test/gplatform/hello51/Makefile			      \
	test/systemc/accum.fdl				      \
	test/systemc/accum_sc.cxx			      \
	test/systemc/Makefile				      \
	test/java/avrora1/Makefile			      \
	test/java/avrora1/simple.c			      \
	test/java/avrora1/module.fdl			      \
	test/java/avrora1/hwsw.java			      \
	test/java/euclid/Makefile			      \
	test/java/euclid/euclid.fdl			      \
	test/java/euclid/euclid.java			      \
	test/java/Makefile				      \
	test/java/counter1/Makefile			      \
	test/java/counter1/counter1.java		      \
	test/java/counter1/counter1.fdl			      \
	test/java/counter2/counter2.fdl			      \
	test/java/counter2/Makefile			      \
	test/java/counter2/counter2.java		      \
	test/java/counter3/Makefile			      \
	test/java/counter3/counter3.fdl			      \
	test/java/counter3/counter3.java		      \
	test/java/avrora2/Makefile			      \
	test/java/avrora2/simple.c			      \
	test/java/avrora2/module.fdl			      \
	test/java/avrora2/hwsw.java			      \
	test/gezel/Makefile				      \
	test/gezel/scripted.fdl				      \
	test/gezel/ipchecksum.fdl			      \
	test/gezel/bresen.fdl				      \
	test/gezel/euclid.fdl				      \
	test/gezel/aes.fdl				      \
	test/gezel/ramblock.fdl				      \
	test/gezel/gf8inv.fdl				      \
	test/gezel/lfsr.fdl				      \
	test/gezel/toggle.fdl				      \
	test/gezel/ramblock2.fdl			      \
        test/Makefile.cfg                                     \
	po/Makefile.in.in

ACLOCAL_AMFLAGS = -I m4
