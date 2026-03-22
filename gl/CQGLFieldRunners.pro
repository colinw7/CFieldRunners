APPNAME = CQGLFieldRunners

BUILD_DIR = $$PWD

include($$(MAKE_DIR)/qt_app.mk)

QMAKE_CXXFLAGS += \
-DBUILD_DIR=\"$$BUILD_DIR\"

QT += opengl

SOURCES += \
main.cpp \
App.cpp \
Toolbar.cpp \
Canvas.cpp \
FieldRunners.cpp \
HUD.cpp \
Axes.cpp \
StartUp.cpp \
GameOver.cpp \
Camera.cpp \
Font.cpp \
GeomObject.cpp \
ShaderProgram.cpp \
CQGLTexture.cpp \

HEADERS += \
App.h \
Toolbar.h \
Canvas.h \
HUD.h \
Axes.h \
StartUp.h \
GameOver.h \
Camera.h \
Font.h \
GeomObject.h \
ShaderProgram.h \
CQGLTexture.h \

INCLUDEPATH += \
$(INC_DIR)/CFieldRunners \
$(INC_DIR)/CAStar \
$(INC_DIR)/CGLCamera \
$(INC_DIR)/CQGLUtil \
$(INC_DIR)/CQIconButton \
$(INC_DIR)/CQStyle \
$(INC_DIR)/CQImage \
$(INC_DIR)/CQPixmapCache \
$(INC_DIR)/CQUtil \
$(INC_DIR)/CGeometry3D \
$(INC_DIR)/CImportModel \
$(INC_DIR)/CMaterial \
$(INC_DIR)/CGeom3DType \
$(INC_DIR)/CClipSide \
$(INC_DIR)/CImageLib \
$(INC_DIR)/CFont \
$(INC_DIR)/CFile \
$(INC_DIR)/CFileType \
$(INC_DIR)/CLineDash \
$(INC_DIR)/CUtil \
$(INC_DIR)/CMath \
$(INC_DIR)/CRGBA \
$(INC_DIR)/COS \

unix:LIBS += \
-L$(LIB_DIR) \
-lCFieldRunners \
-lCQGLUtil \
-lCQIconButton \
-lCQPixmapCache \
-lCQStyle \
-lCQImage \
-lCQUtil \
-lCImportModel \
-lCGeometry3D \
-lCVoxel \
-lCCSG \
-lCXML \
-lCJson \
$$CIMAGE_LIBS \
-lCFile \
-lCMath \
-lCStrParse \
-lCStrUtil \
-lCPrintF \
-lCEncode \
-lCRGBUtil \
-lCOS \
-lGLU -lz
