
dnl Process this file with autoconf to produce a configure script.
AC_INIT(standalone/fdlsim.cxx)
AM_INIT_AUTOMAKE(gezel, 2.5)

#AM_CONFIG_HEADER([gplatform/atmega_config.h])

AC_PREFIX_DEFAULT("`pwd`/build")

#Sachin path info
confdir="${datadir}/gplatform/config"
devicedir="${datadir}/gplatform/devices"
AC_SUBST(confdir, ${confdir})
AC_SUBST(devicedir, ${devicedir})

#Sachin end

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_LIBTOOL

# Check for gmp
AC_CHECK_LIB(gmp, __gmpz_init, , [AC_MSG_ERROR([GNU Multiprecision Library is required])])

AC_ARG_ENABLE(standalone,
    [  --enable-standalone            build standalone simulator/ code generator [default=no]],
    standaloneenable=yes,standaloneenable=no)

# Check for xml
#AM_PATH_XML2(,,AC_MSG_ERROR([*** libxml2 not installed - please install first ***]))

AM_CONDITIONAL([BUILDSTANDALONE], [test "$standaloneenable" = yes])

AC_ARG_ENABLE(java,
    [  --enable-java                 build JAVA cosimulation interface [default=no]],
    javaenable=yes,javaenable=no)

AM_CONDITIONAL([BUILDJAVA], [test "$javaenable" = yes])

AC_ARG_ENABLE(gplatform,
    [  --enable-gplatform             build multi-core platform simulator [default=no]],
    gplatformenable=yes,gplatformenable=no)

AM_CONDITIONAL([BUILDGPLATFORM], [test "$gplatformenable" = yes])

needsimit=no;
if test "$gplatformenable" != "no"; then
   needsimit=yes;
fi;
if test "$needsimit" != "no"; then
   simitsrc=
   AC_ARG_WITH(simit,
     [  --with-simit           Specify location of SimIt-ARM installation],
     [simitsrc="$withval"])
fi;

if test "$needsimit" != "no"; then
   CPPFLAGS="$CPPFLAGS -I$simitsrc/include";
   LDFLAGS="$LDFLAGS -L$simitsrc/lib";
fi;

needsimulavr=no;
if test "$gplatformenable" != "no"; then
   needsimulavr=yes;
fi;
if test "$needsimulavr" != "no"; then
   simulavrsrc=
   AC_ARG_WITH(simulavr,
     [  --with-simulavr           Specify location of SimulAVR installation],
     [simulavrsrc="$withval"])
fi;

if test "$needsimit" != "no"; then
   CPPFLAGS="$CPPFLAGS -I$simulavrsrc/include";
   LDFLAGS="$LDFLAGS -L$simulavrsrc/lib";
fi;

AC_ARG_ENABLE(simitsfu,
    [  --enable-simitsfu              include support for Special Function Units in SimIt-ARM  [default=no]],
    simitsfu=yes,simitsfu=no)

AM_CONDITIONAL([SUPPORTSIMITSFU], [test "$simitsfu" = yes])

AC_ARG_ENABLE(systemccosim,
    [  --enable-systemccosim          build SystemC cosimulator [default=no]],
    systemccosimenable=yes,systemccosimenable=no)

AM_CONDITIONAL([BUILDSYSTEMCCOSIM], [test "$systemccosimenable" = yes])
    
if test "$systemccosimenable" != "no"; then
   systemclib=
   AC_ARG_WITH(systemc-lib,
     [  --with-systemc-lib           Specify location of SystemC library (e.g. /opt/systemc/lib-linux)],
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

CXX_COMP=`basename $CXX`

AC_ARG_ENABLE(debug,
    [  --enable-debug                 Compile with debug information [default=no]],
    debugenable=yes,debugenable=no)

CXXFLAGS="-Wall -O3 -fomit-frame-pointer"
if test "$debugenable" != "no"; then
   CXXFLAGS="-Wall -g -pg"
fi;

#Sachin
AC_SUBST(SOEXT, "so")
AC_SUBST(SOCFLAGS, "-fPIC")
AC_SUBST(CC_SHAREDFLAG, "-shared")
AC_DEFINE_UNQUOTED(SOEXT,"${SOEXT}",[What this platform's library file extension is])
#Sachin

CXXFLAGS="$CXXFLAGS $XCXXFLAGS"

dnl Checks for typedefs, structures, and compiler characteristics.

# see if we can use ostringstream and sstream
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE([#include <sstream>
using std::ostringstream;
],[ostringstream message; message << "Hello"; return 0;],
AC_DEFINE(HAVE_SSTREAM,,[define if the compiler has stringstream]), )

# AC_OUTPUT(Makefile gezel/Makefile ipblock/Makefile systemc/Makefile  po/Makefile.in m4/Makefile java/Makefile standalone/Makefile gplatform/Makefile)
# AC_CONFIG_FILES([gplatform/devices/Makefile])
AC_OUTPUT(Makefile gezel/Makefile ipblock/Makefile systemc/Makefile  m4/Makefile java/Makefile standalone/Makefile gplatform/Makefile ghost/Makefile)
