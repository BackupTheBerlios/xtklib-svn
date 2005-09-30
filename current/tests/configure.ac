AC_INIT([xtktests],[0.1.0],[xshadow@email.it])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_SIGNAL

#AC_CHECK_LIB(xtk,xtk::xString::getFormat,[XTKLIB=-lxtk],[AC_MSG_ERROR(xtklib is not installed)])
#AC_SUBST([XTKLIB])

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
		LDFLAGS="$LDFLAGS -g"
		;;
*)		DEBUG_FLAGS=""
		
		#remove -g in nondebug
		CFLAGS="`echo $CFLAGS | sed "s/-g//g"`"
		CXXFLAGS="`echo $CXXFLAGS | sed "s/-g//g"`"
		;;
esac


CXXFLAGS="$CXXFLAGS $DEBUG_FLAGS"


AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
