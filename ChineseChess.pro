#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T17:38:13
#
#-------------------------------------------------

QT       += core gui widgets
QT += network
QT       += multimedia


TARGET = ChineseChess
TEMPLATE = app

ICON = ./images/chessIcon.icns

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

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    chesspiece.cpp \
    chessboard.cpp \
    netserver.cpp \
    chesspos.cpp \
    gamecenter.cpp \
    graphicsscene.cpp \
    pieceitem.cpp \
    waitdialog.cpp \
    connectdialog.cpp

HEADERS += \
        mainwindow.h \
    chesspiece.h \
    chessboard.h \
    netserver.h \
    chesspos.h \
    gamecenter.h \
    graphicsscene.h \
    pieceitem.h \
    waitdialog.h \
    connectdialog.h

FORMS += \
        mainwindow.ui \
    waitdialog.ui \
    connectdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    chessresource.qrc
