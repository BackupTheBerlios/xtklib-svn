SUBDIRS = gtk2

lib_LIBRARIES = libxtkwidgets.a

libxtkwidgets_a_CPPFLAGS = $(WIDGETSDEPS_CFLAGS)
libxtkwidgets_a_LIBADD = \
	gtk2/libxtkwidgets_gtk2_a-button_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-container_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-layout_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-widget_gtk2.o \
	gtk2/libxtkwidgets_gtk2_a-window_gtk2.o
       
libxtkwidgets_a_SOURCES = \
	button.cpp \
	container.cpp \
	font.cpp \
	frame.cpp \
	layout.cpp \
	widget.cpp \
	widgetevent.cpp \
	widgeteventgenerator_internal.cpp \
	window.cpp 
