QT += gui widgets
TARGET = fromnow
TEMPLATE = lib
DEFINES += LIBFROMNOW_LIBRARY

SOURCES += \
    fn_widgets.cpp

HEADERS += \
        libfromnow_global.h \ 
    fn_widgets.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
