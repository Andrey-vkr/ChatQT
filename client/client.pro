QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


SOURCES += \
    ChatWindow.cpp \
    ClientQt.cpp \
    InputValidator.cpp \
    LoginWindow.cpp \
    RegistrationWindow.cpp \
    ScreenController.cpp \
    WrapperSHA1.cpp \
    main.cpp

HEADERS += \
    ChatWindow.h \
    ClientQt.h \
    InputValidator.h \
    LoginWindow.h \
    RegistrationWindow.h \
    ScreenController.h \
    Sha1.hpp \
    WrapperSHA1.h

FORMS += \
    ChatWindow.ui \
    LoginWindow.ui \
    RegistrationWindow.ui \
    ScreenController.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
