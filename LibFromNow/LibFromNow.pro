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
}
