#-------------------------------------------------
#
# Project created by QtCreator 2011-07-27T21:25:30
#
#-------------------------------------------------

QT       += core gui xml phonon

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Winline -Winvalid-pch \
    -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual -Wformat=2 \
    -Wshadow -pedantic # -Weffc++

QMAKE_CFLAGS += -std=c11

QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

TARGET = ydpd
TEMPLATE = app

Ydpd.path = /usr/local/bin

INSTALLS += ydpd

SOURCES += main.cpp\
    libydpdict/ydpdict.c \
    dictmanager.cpp \
    config.cpp \
    configmanager.cpp \
    dict.cpp \
    configtabdialog.cpp \
    pathswidget.cpp \
    dictinstance.cpp \
    ydpd.cpp

HEADERS  += \
    libydpdict/ydpdict.h \
    dictmanager.h \
    config.h \
    configmanager.h \
    dict.h \
    configtabdialog.h \
    pathswidget.h \
    ydptypes.h \
    dictinstance.h \
    ydpd.h

FORMS    += \
    startupForm.ui \
    pathsForm.ui \
    historyForm.ui \
    ydpdForm.ui

RESOURCES += \
    Ydpd.qrc
    
RC_FILE = ydpd.rc

OTHER_FILES +=
