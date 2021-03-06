#-------------------------------------------------
#
# Project created by QtCreator 2020-05-30T22:15:46
#
#-------------------------------------------------

QT       += core gui widgets multimedia charts

TARGET = Spectrogrammer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += qwt

include(/Users/tameraktekin/Qt/qwt-6.1.5/qwt.prf) # Note: This line solved qwt problem.

INCLUDEPATH += "/usr/local/include"

LIBS += -framework CoreAudio
LIBS += -framework AudioToolbox
LIBS += -L"/usr/local/lib" -lfftw3 -lfftw3f

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        optionsmenu.cpp \

HEADERS += \
        mainwindow.h \
        optionsmenu.h \

FORMS += \
        mainwindow.ui \
        optionsmenu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
