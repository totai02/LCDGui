TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    menuitem.cpp \
    menubar.cpp \
    button.cpp \
    keyboard.cpp \
    imagemenuitem.cpp \
    dialog.cpp \
    textview.cpp \
    editdialog.cpp \
    numbermenuitem.cpp \
    wifiscanner.cpp

LIBS += -lpcd8544 -lwiringPi -lstdc++fs

HEADERS += \
    menuitem.h \
    menubar.h \
    button.h \
    keyboard.h \
    imagemenuitem.h \
    dialog.h \
    define.h \
    textview.h \
    editdialog.h \
    numbermenuitem.h \
    wifiscanner.h
