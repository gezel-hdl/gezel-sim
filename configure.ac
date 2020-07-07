AC_INIT([gezel-sim], [2.5], [https://github.com/gezel-hdl/gezel-sim/issues])
AC_CONFIG_SRCDIR([gezel/fdlsym.cxx])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_MACRO_DIRS([m4])

confdir="${datadir}/gplatform/config"
devicedir="${datadir}/gplatform/devices"
AC_SUBST(confdir, ${confdir})
AC_SUBST(devicedir, ${devicedir})

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
AM_PROG_LEX
AC_PROG_YACC
LT_INIT([pic-only])

if test -z "$LEX"; then
    AC_MSG_ERROR([lex/flex is required])
fi;

if test -z "$YACC"; then
    AC_MSG_ERROR([yacc/bison is required])
fi;

if test "$pic_mode" != "yes"; then
    AC_MSG_WARN([gezel will not support modules])
fi;

# Check for gmp
AC_CHECK_LIB([gmp], [__gmpz_init], [], [AC_MSG_ERROR([GNU Multiprecision Library is required])])

AX_ARG_DISABLE(standalone,
    AS_HELP_STRING([--disable-standalone], [build standalone simulator/ code generator [default=yes]]))

AM_CONDITIONAL([BUILDSTANDALONE], [test "$enable_standalone" = yes])

AC_ARG_ENABLE(java,
    AS_HELP_STRING([--enable-java], [build JAVA cosimulation interface [default=no]]))

if test "$enable_java" = yes; then
    AX_PROG_JAVAC
    AX_PROG_JAVA
    AX_PROG_JAVAH
fi

AM_CONDITIONAL([BUILDJAVA], [test "$enable_java" = yes])

AC_ARG_ENABLE(gplatform,
    AS_HELP_STRING([--enable-gplatform], [build multi-core platform simulator [default=no]]))

AM_CONDITIONAL([BUILDGPLATFORM], [test "$enable_gplatform" = yes])

needsimit=no
if test "$enable_platform" = "yes"; then
   needsimit=yes
fi;
if test "$needsimit" = "yes"; then
   simitsrc=
   AC_ARG_WITH(simit,
     AS_HELP_STRING([--with-simit], [Specify location of SimIt-ARM installation]),
     [simitsrc="$withval"])
fi;

if test "$needsimit" = "yes"; then
   CPPFLAGS="$CPPFLAGS -I$simitsrc/include";
   LDFLAGS="$LDFLAGS -L$simitsrc/lib";
fi;

needsimulavr=no;
if test "$gplatformenable" = "yes"; then
   needsimulavr=yes;
fi;
if test "$needsimulavr" = "yes"; then
   simulavrsrc=
   AC_ARG_WITH(simulavr,
     AS_HELP_STRING([--with-simulavr], [Specify location of SimulAVR installation]),
     [simulavrsrc="$withval"])
fi;

if test "$needsimit" = "yes"; then
   CPPFLAGS="$CPPFLAGS -I$simulavrsrc/include";
   LDFLAGS="$LDFLAGS -L$simulavrsrc/lib";
fi;

AC_ARG_ENABLE(simitsfu,
    AS_HELP_STRING([--enable-simitsfu], [include support for Special Function Units in SimIt-ARM  [default=no]]))

AM_CONDITIONAL([SUPPORTSIMITSFU], [test "$enable_simitsfu" = yes])

AC_ARG_ENABLE(systemccosim,
    AS_HELP_STRING([--enable-systemccosim], [build SystemC cosimulator [default=no]]))

AM_CONDITIONAL([BUILDSYSTEMCCOSIM], [test "$enable_systemccosim" = yes])
    
if test "$enable_systemccosim" = "yes"; then
   systemclib=
   AC_ARG_WITH(systemc-lib,
     AS_HELP_STRING([--with-systemc-lib], [Specify location of SystemC library (e.g. /opt/systemc/lib-linux)]),
     [systemclib="$withval"])
   CPPFLAGS="$CPPFLAGS -I$systemclib/../include";
   LDFLAGS="$LDFLAGS -L$systemclib";
fi;

needlibdl=no;
if test "$host_os" != "cygwin"; then
  needlibdl=yes;
fi;
AM_CONDITIONAL([LINKLIBDL], [test "$needlibdl" = yes])

dnl Checks for libraries.

dnl Checks for header files.

AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [Compile with debug information [default=no]]))

XCXXFLAGS="-Wall -O3 -fomit-frame-pointer"
if test "$enable_debug" = "yes"; then
   XCXXFLAGS="-Wall -g -pg"
fi;

CXXFLAGS="$CXXFLAGS $XCXXFLAGS"

dnl Checks for typedefs, structures, and compiler characteristics.

AC_CONFIG_FILES([
    Makefile
    gezel/Makefile
    ipblock/Makefile
    systemc/Makefile
    java/Makefile
    standalone/Makefile
    gplatform/Makefile
    ghost/Makefile
    test/Makefile
    test/gezel/Makefile
    test/gezel/scripted.fdl:test/gezel/scripted.fdl.in
    test/gplatform/Makefile
    test/gplatform/aes8051/Makefile
    test/gplatform/aesarm/Makefile
    test/gplatform/arm2arm/Makefile
    test/gplatform/armsfu/Makefile
    test/gplatform/armsystemc/Makefile
    test/gplatform/avr-interrupt/Makefile
    test/gplatform/avr-timer-spi/Makefile
    test/gplatform/block8051/Makefile
    test/gplatform/blockarm/Makefile
    test/gplatform/cordicfsl/Makefile
    test/gplatform/cyclecount/Makefile
    test/gplatform/diblaze/Makefile
    test/gplatform/euclid/Makefile
    test/gplatform/fsl2/Makefile
    test/gplatform/fslinterface/Makefile
    test/gplatform/gfmul/Makefile
    test/gplatform/hello51/Makefile
    test/gplatform/helloarm/Makefile
    test/gplatform/pblaze/Makefile
    test/gplatform/ramprobe/Makefile
    test/gplatform/ssidehsk/Makefile
    test/gplatform/test-avr/Makefile
    test/java/Makefile
    test/java/counter1/Makefile
    test/java/counter2/Makefile
    test/java/counter3/Makefile
    test/java/euclid/Makefile
    test/ipblock/Makefile
    test/ipblock/lutram/Makefile
])
AC_OUTPUT
