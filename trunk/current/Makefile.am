if USE_WIDGETS
WIDGETS_PC=xtkwidgets.pc
endif

SUBDIRS = src include

EXTRA_DIST = xtkbase.pc.in $(WIDGETS_PC).in
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xtkbase.pc $(WIDGETS_PC)
