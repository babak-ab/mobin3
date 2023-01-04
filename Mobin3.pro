QT += quick qml multimedia widgets serialport
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/utils/utils.pri)

SOURCES += \
        appcontrol.cpp \
        buttonmodel.cpp \
        main.cpp \
        serialcontrol.cpp \
        videoadapter.cpp \
        videocapture.cpp \
        videorecord.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    appcontrol.h \
    buttonmodel.h \
    global.h \
    serialcontrol.h \
    videoadapter.h \
    videocapture.h \
    videorecord.h

DISTFILES += \
    CustomButton.qml \
    utils/utils.pri

CONFIG += link_pkgconfig \
            c++11

PKGCONFIG += gstreamer-1.0 \
        glib-2.0 \
        gobject-2.0 \
        gio-2.0


win32{
win32: LIBS += -L'C:/Program Files/OpenCV3.4.16/lib/' -lopencv_world3416

INCLUDEPATH += 'C:/Program Files/OpenCV3.4.16/include'
DEPENDPATH += 'C:/Program Files/OpenCV3.4.16/include'
}
