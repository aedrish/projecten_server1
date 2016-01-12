QZMQ_DIR = $$PWD/../../qzmq
COMMON_DIR = $$PWD/../../common

INCLUDEPATH += $$QZMQ_DIR/src
include($$QZMQ_DIR/src/src.pri)

INCLUDEPATH += $$COMMON_DIR
DEFINES += NO_IRISNET

HEADERS += \
	$$COMMON_DIR/processquit.h \
	$$COMMON_DIR/tnetstring.h \
	$$COMMON_DIR/httpheaders.h \
	$$COMMON_DIR/zhttprequestpacket.h \
	$$COMMON_DIR/zhttpresponsepacket.h \
	$$COMMON_DIR/bufferlist.h \
	$$COMMON_DIR/log.h \
	$$COMMON_DIR/layertracker.h

SOURCES += \
	$$COMMON_DIR/processquit.cpp \
	$$COMMON_DIR/tnetstring.cpp \
	$$COMMON_DIR/httpheaders.cpp \
	$$COMMON_DIR/zhttprequestpacket.cpp \
	$$COMMON_DIR/zhttpresponsepacket.cpp \
	$$COMMON_DIR/bufferlist.cpp \
	$$COMMON_DIR/log.cpp \
	$$COMMON_DIR/layertracker.cpp

HEADERS += \
	$$PWD/m2requestpacket.h \
	$$PWD/m2responsepacket.h \
	$$PWD/../version.h \
	$$PWD/app.h

SOURCES += \
	$$PWD/m2requestpacket.cpp \
	$$PWD/m2responsepacket.cpp \
	$$PWD/app.cpp \
	$$PWD/main.cpp
