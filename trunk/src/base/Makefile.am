libbase_a_srcs = \
	application.cpp \
	arraylist.cpp \
	avlmap.cpp \
	bufferedcharstream.cpp \
	bufferedstream.cpp \
	bytearraystream.cpp \
	charset.cpp \
	charstream.cpp \
	color.cpp \
	datastream.cpp \
	exception.cpp \
	execinfo.c \
	file.cpp \
	hashmap.cpp \
	inetaddress.cpp \
	linkedlist.cpp \
	logging.cpp \
	memory.cpp \
	messagestream.cpp \
	object.cpp \
	preferences.cpp \
	process.cpp \
	random.cpp \
	serversocket.cpp \
	socket.cpp \
	stacktrace.cpp \
	stacktraverse.c \
	stream.cpp \
	string.cpp \
	system.cpp \
	testing.cpp \
	thread.cpp \
	threadgroup.cpp \
	time.cpp \
	triggerableevent.cpp \
	typewrappers.cpp \
	utilities.cpp \
	zlibstream.cpp

if COND_ENBL_DLL
lib_LTLIBRARIES=libxtkbase.la
libxtkbase_la_SOURCES = $(libbase_a_srcs)
libxtkbase_la_CXXFLAGS = $(AM_CXXFLAGS)
libxtkbase_la_CFLAGS = $(AM_CFLAGS)
else
lib_LIBRARIES=libxtkbase.a
libxtkbase_a_SOURCES = $(libbase_a_srcs)
endif