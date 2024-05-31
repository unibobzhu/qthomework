QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    attach.cpp \
    desktopbk.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow3.cpp

HEADERS += \
    attach.h \
    desktopbk.h \
    mainwindow.h \
    mainwindow3.h \
    weatherdata.h

FORMS += \
    attach.ui \
    mainwindow.ui \
    mainwindow3.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bk.qrc
