QT       += core gui widgets
TARGET = FromNow
TEMPLATE = app
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    types.h

INCLUDEPATH += ../LibFromNow
LIBS += -L../LibFromNow -lfromnow
