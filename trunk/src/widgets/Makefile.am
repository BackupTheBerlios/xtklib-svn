SUBDIRS = gtk2
lib_LTLIBRARIES = libxtkwidgets.la

libxtkwidgets_la_CPPFLAGS = $(WIDGETSDEPS_CFLAGS)
libxtkwidgets_la_LIBADD = \
	gtk2/libxtkwidgets_gtk2_a-button_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-container_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-layout_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-widget_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-window_gtk2.o
       
libxtkwidgets_la_SOURCES = \
	button.cpp \
	container.cpp \
	font.cpp \
	frame.cpp \
	label.cpp \
	layout.cpp \
	panel.cpp \
	widget.cpp \
	widgetevent.cpp \
	widgeteventgenerator_internal.cpp \
	window.cpp 
