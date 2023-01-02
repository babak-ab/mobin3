#TARGET = utils
#include(../../Simorghlibrary.pri)

#DEFINES += UTILS_LIBRARY

INCLUDEPATH += $$PWD

SOURCES += \
           $$PWD/utils.cpp \
           $$PWD/cursor.cpp \
           $$PWD/ringqueue.cpp \

HEADERS += \
           $$PWD/utils.h \
           $$PWD/cursor.h \
           $$PWD/ringqueue.h \
           $$PWD/ringqueue_p.h \

