AC_INIT([xtktests],[0.1.0],[xshadow@email.it])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CANONICAL_HOST

AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET

dnl ==========================================================================
dnl	== Checking for dependencies
dnl ==========================================================================

dnl This is disabled by default becouse we use the local libs and includes
dnl PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 xtkbase >= 0.1.0 xtkwidgets >= 0.1.0)
PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

DEPS_CFLAGS_A="-I../include/"
DEPS_LIBS_A="-L../src/base -L../src/widgets -lxtkbase -lxtkwidgets"
AC_SUBST(DEPS_CFLAGS_A)
AC_SUBST(DEPS_LIBS_A)


dnl ==========================================================================

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_SIGNAL

dnl ==========================================================================


dnl Choose system target
case $host in
  *-*-mingw32* | *-*-windows*)
    CXXFLAGS="$CXXFLAGS -D__WINDOWS__"
    ;;
  *-*-linux*)
    CXXFLAGS="$CXXFLAGS -D__UNIX__"
    ;;
esac

dnl Removing -fomit since it disable stacktracing
CFLAGS="`echo $CFLAGS | sed "s/-fomit-frame-pointer//g"`"
CXXFLAGS="`echo $CXXFLAGS | sed "s/-fomit-frame-pointer//g"`"

dnl default flags
CXXFLAGS="$CXXFLAGS -frtti -fexceptions -D__WIDGETS__"

dnl ==========================================================================

AC_ARG_ENABLE(debug,[  --enable-debug		Turn on debugging],USE_DEBUG="$enableval")

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
dnl ==========================================================================

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
