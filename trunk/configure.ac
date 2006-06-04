AC_INIT([xtklib],[0.2.0],[xshadow@email.it])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall foreign])
AM_MAINTAINER_MODE
AC_PREFIX_DEFAULT(/usr)

dnl ==========================================================================

AC_PROG_CC
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL

dnl ==========================================================================
dnl	== Checking for dependencies
dnl ==========================================================================
dnl PKG_CHECK_MODULES(WIDGETSDEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
dnl AC_SUBST(WIDGETSDEPS_CFLAGS)

dnl ==========================================================================

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/ioctl.h unistd.h execinfo.h sched.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(open64 stat64 lseek64 pthread_yield sched_yield)

dnl ==========================================================================
dnl Check if iconv uses const in prototype declaration
dnl ==========================================================================
AC_CACHE_CHECK(for const presence in iconv(), ac_cv_iconv_const,
  [AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
      [[
        #include <stdlib.h>
        #include <iconv.h>
      ]],
      [[
        iconv_t cd = iconv_open("ASCII", "ASCII");
        char * inbuf = "ABCDE";
        char * outbuf = new char[6];
        size_t inbytesleft, outbytesleft;
        iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
      ]]
   )],
   [ac_cv_iconv_const=0],
   [ac_cv_iconv_const=1]
  )]
)

AC_DEFINE_UNQUOTED([ICONV_CONST], $ac_cv_iconv_const,
  [Define if there's a const in the iconv() signature.])

dnl Choose system target
case "${target_os}" in
"")
	SYS=unknown
	;;
*darwin*)
	SYS=darwin
	CXXFLAGS="$CXXFLAGS -D__UNIX__"
	;;
*openbsd*) 
	SYS=unknown
	;;
*linux*) 
	SYS=unix
	DARWIN=no
	CXXFLAGS="$CXXFLAGS -D__UNIX__"
	;;
*cygwin* | *mingw32*)
	SYS=win
	CXXFLAGS="$CXXFLAGS -D__WINDOWS__"
	;;
*solaris*)
	SYS=unknown
esac



dnl Removing -fomit since it disable stacktracing
CFLAGS="`echo $CFLAGS | sed "s/-fomit-frame-pointer//g"`"
CXXFLAGS="`echo $CXXFLAGS | sed "s/-fomit-frame-pointer//g"`"

dnl default flags
CXXFLAGS="$CXXFLAGS -frtti -fexceptions"

dnl ==========================================================================

dnl Features enabled
AC_ARG_ENABLE(debug,[  --enable-debug		Turn on debugging],USE_DEBUG="$enableval",USE_DEBUG="no")
AC_ARG_ENABLE(widgets,[  --enable-widgets		Turn on widgets module],USE_WIDGETS="$enableval",USE_WIDGETS="no")


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


case "$USE_WIDGETS" in
yes)	CXXFLAGS="$CXXFLAGS -D__WIDGETS__" 
		if test "$SYS" == "unix"; then
			dnl	== Checking for dependencies
			PKG_CHECK_MODULES(WIDGETSDEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
			AC_SUBST(WIDGETSDEPS_CFLAGS)
		fi
		;;
*)		CXXFLAGS="$CXXFLAGS" 
		;;
esac
AM_CONDITIONAL(USE_WIDGETS,test x$USE_WIDGETS=xyes)

CXXFLAGS="$CXXFLAGS $DEBUG_FLAGS"

dnl ==========================================================================

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile 
				src/Makefile 
				src/base/Makefile 
				src/widgets/Makefile 
				src/widgets/gtk2/Makefile 
				include/Makefile 
				include/xtk/Makefile 
				include/xtk/base/Makefile 
				include/xtk/widgets/Makefile
				xtkbase.pc
				xtkwidgets.pc
				])
AC_OUTPUT

dnl ==========================================================================

echo
echo "  Configured xtklib for '${target_os}'."
echo
echo "  xtklib enabled options:"
echo 
echo "  Prefix where xtklib should be installed                    ${prefix:-none}"
echo "  xtklib compibled in debug mode?                            ${USE_DEBUG:-no}"
echo "  Widgets module enabled?                                    ${USE_WIDGETS:-no}"
echo
