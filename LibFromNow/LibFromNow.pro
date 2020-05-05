QT += gui widgets
TARGET = fromnow
TEMPLATE = lib
DEFINES += LIBFROMNOW_LIBRARY

SOURCES += \
	fn_widgets.cpp \
	fn_entities.cpp

HEADERS += \
	libfromnow_global.h \
	fn_widgets.h \
	fn_entities.h \
	fn_types.h

unix {
	target.path = /usr/lib
	INSTALLS += target
	QMAKE_CXXFLAGS += -Wno-covered-switch-default
}

win32 {
	CONFIG += skip_target_version_ext
	CONFIG -= debug_and_release debug_and_release_target
}
