QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    CommandHandler.cpp \
    Database.cpp \
    ServerQt.cpp \
    WrapperSHA1.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CommandHandler.h \
    Database.h \
    ServerQt.h \
    Sha1.hpp \
    WrapperSHA1.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



