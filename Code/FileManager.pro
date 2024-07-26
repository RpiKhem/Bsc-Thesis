QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ebookconvthread.cpp \
    filepathmodel.cpp \
    formatwindow.cpp \
    foundfileswindow.cpp \
    main.cpp \
    maze.cpp \
    model.cpp \
    view.cpp \
    waitdialog.cpp

HEADERS += \
    ebookconvthread.h \
    filepathmodel.h \
    formatwindow.h \
    foundfileswindow.h \
    maze.h \
    model.h \
    view.h \
    waitdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
