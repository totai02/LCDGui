TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    menuitem.cpp \
    menubar.cpp \
    button.cpp \
    keyboard.cpp \
    wifiscanner.cpp \
    imagemenuitem.cpp \
    dialog.cpp \
    textview.cpp

LIBS += -lpcd8544 -lwiringPi -lstdc++fs -O3 -liw

HEADERS += \
    menuitem.h \
    menubar.h \
    button.h \
    keyboard.h \
    wifiscanner.h \
    imagemenuitem.h \
    dialog.h \
    define.h \
    textview.h
