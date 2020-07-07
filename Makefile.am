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

SUBDIRS = gezel ipblock ghost $(SYSTEMCCOSIMDIR) $(JAVADIR) $(GPLATFORMDIR) $(STANDALONEDIR) test

# find test -name '*' -type f | grep -v '.svn'

EXTRA_DIST = \
	BUGS \
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
	test/gplatform/xilinxipifreg_sub64/ipreg_example.fdl \
	test/gplatform/xilinxipifreg_sub64/ipreg_drive.c \
	test/gplatform/xilinxipifreg_sub64/Makefile \
	test/gplatform/xilinxipifreg_sub64/PLB_EDK92 \
	test/gplatform/xilinxipifmem_gcd/OPB_EDK8 \
	test/gplatform/xilinxipifmem_gcd/ipmem_drive.c \
	test/gplatform/xilinxipifmem_gcd/Makefile \
	test/gplatform/xilinxipifmem_gcd/ipmem_example.fdl \
	test/gplatform/xilinxipifreg_gcd/ipreg_example.fdl \
	test/gplatform/xilinxipifreg_gcd/edk92_gcdmm.zip \
	test/gplatform/xilinxipifreg_gcd/ipreg_drive.c \
	test/gplatform/xilinxipifreg_gcd/Makefile \
	test/gplatform/xilinxipifreg_gcd/PLB_EDK92 \
	test/gplatform/xilinxfsl_cordic/cordic_fixp.c \
	test/gplatform/xilinxfsl_cordic/fsl.fdl \
	test/gplatform/xilinxfsl_cordic/Makefile \
	test/gplatform/xilinxfsl_cordic/PLB_EDK92 \
	test/gplatform/xilinxfsl_cordic/edk92_cordicfsl.zip \
	test/gplatform/xilinxipiffifo_gcd/fifodrive.c \
	test/gplatform/xilinxipiffifo_gcd/OPB_EDK8 \
	test/gplatform/xilinxipiffifo_gcd/user.fdl \
	test/gplatform/xilinxipiffifo_gcd/Makefile \
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
	test/java/counter1/Makefile

# \
#	po/Makefile.in.in

ACLOCAL_AMFLAGS = -I m4
