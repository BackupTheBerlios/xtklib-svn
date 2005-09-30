AC_INIT([xtklib],[0.1.0],[xshadow@email.it])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_PREFIX_DEFAULT(/usr)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_SIGNAL

CXXFLAGS="$CXXFLAGS -frtti -fexceptions"

dnl Here must switch ${target}
CXXFLAGS="-D__UNIX__"

AC_ARG_ENABLE(debug,[  --enable-debug		Turn on debugging],USE_DEBUG="$enableval")

# Removing -fomit since it disable stacktracing
CFLAGS="`echo $CFLAGS | sed "s/-fomit-frame-pointer//g"`"
CXXFLAGS="`echo $CXXFLAGS | sed "s/-fomit-frame-pointer//g"`"

case "$USE_DEBUG" in
yes)	DEBUG_FLAGS="-Wall -Wno-unused-parameter -g -D__DEBUG__"
		# No -O2
		CFLAGS="`echo $CFLAGS | sed "s/-O2//g"`"
		CXXFLAGS="`echo $CXXFLAGS | sed "s/-O2//g"`"
		;;
*)		DEBUG_FLAGS=""
		
		#remove -g in nondebug
		CFLAGS="`echo $CFLAGS | sed "s/-g//g"`"
		CXXFLAGS="`echo $CXXFLAGS | sed "s/-g//g"`"
		;;
esac


CXXFLAGS="$CXXFLAGS $DEBUG_FLAGS"


AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile src/Makefile src/base/Makefile include/Makefile include/xtk/Makefile include/xtk/base/Makefile])
AC_OUTPUT
