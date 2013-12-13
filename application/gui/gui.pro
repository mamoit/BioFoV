#-------------------------------------------------
#
# Project created by QtCreator 2013-11-27T14:18:19
#
#-------------------------------------------------

! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

win32 {
    #LIBS += -LC:/qt/OpenCV246_bin/install/lib -llopencv_core246 -llopencv_highgui246
    LIBS += -L C:\FIXME\opencv\build\install\lib
    LIBS += -lopencv_core245 -lopencv_highgui245 -lopencv_video245
    LIBS += -lopencv_imgproc245
    INCLUDEPATH += C:\FIXME\opencv\build\install\include
    LIBS += -L..\framework\release -lframework
}

# reset sources
SOURCES += \
    about.cpp \
    mainwindow.cpp \
    videoplayer.cpp \
    videoitem.cpp \
    eventitem.cpp \
    splitdialog.cpp \
    calibrationdialog.cpp

# reset headers
HEADERS += \
    about.hpp \
    mainwindow.hpp \
    videoplayer.hpp \
    videoitem.hpp \
    eventitem.hpp \
    splitdialog.hpp \
    calibrationdialog.hpp

FORMS    += $$files(*.ui)

RESOURCES += $$files(*.qrc)
unix {
    LIBS += -L../framework -lframework
}
