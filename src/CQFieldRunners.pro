TEMPLATE = app

QT += widgets svg

TARGET = CQFieldRunners

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++17

#CONFIG += debug

# Input
SOURCES += \
CQFieldRunners.cpp \
CFieldRunners.cpp \
CQCanvas2D.cpp \

HEADERS += \
CQFieldRunners.h \
CFieldRunners.h \
CQCanvas2D.h \
CDisplayRange2D.h \
CAStar.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../../CQUtil/include \
../../CUtil/include \
../../CFont/include \
../../CFile/include \
../../CStrUtil/include \
../../CMath/include \
../../COS/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CQUtil/lib \
-L../../CUtil/lib \
-L../../CFont/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../COS/lib \
-lCQUtil \
-lCUtil \
-lCFont \
-lCFile \
-lCMath \
-lCStrUtil \
-lCRegExp \
-lCOS \
-ltre
