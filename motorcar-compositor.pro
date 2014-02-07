QT += gui gui-private core-private compositor

LIBOVRPATH=../thirdPartySource/Oculus/OculusSDK/LibOVR
INCLUDEPATH += "$$LIBOVRPATH/Include" "$$LIBOVRPATH/Src"

RELEASETYPE=Release
SYSARCH=x86_64

LIBS += -L$$LIBOVRPATH/Lib/Linux/$$RELEASETYPE/$$SYSARCH -lovr -ludev -lpthread -lGL -lX11 -lXinerama

DESTDIR = bin
OBJECTS_DIR = bin/.obj
MOC_DIR = bin/.moc
RCC_DIR = bin/.rcc
UI_DIR = bin/.ui

QMAKE_CXXFLAGS += -std=c++11 -DGL_GLEXT_PROTOTYPES

LIBS += -L ../../lib -lGL -lGLU -lglut
#include (../../src/qt-compositor/qt-compositor.pri)
#include(scenegraph/include.pri)

HEADERS += \
    qt/src/textureblitter.h \
    qt/src/qtwaylandmotorcarcompositor.h \
    qt/src/qopenglwindow.h \
    qt/src/opengldata.h \
    motorcar/src/geometry.h \
    motorcar/src/scenegraph/scenegraphnode.h \
    motorcar/src/scenegraph/scenegraph.h \
    motorcar/src/scenegraph/output/glcameranode.h \
    motorcar/src/scenegraph/physicalnode.h \
    motorcar/src/scenegraph/virtualnode.h \
    motorcar/src/scenegraph/ioelement.h \
    motorcar/src/scenegraph/scene.h \
    motorcar/src/scenegraph/output/outputelement.h \
    motorcar/src/scenegraph/output/drawable.h \
    motorcar/src/scenegraph/output/wayland/waylandsurfacenode.h \
    motorcar/src/scenegraph/output/output.h \
    motorcar/src/scenegraph/foo.h \
    motorcar/src/motorcar.h \
    motorcar/src/scenegraph/output/wayland/waylandsurface.h \
    qt/src/qtwaylandmotorcarsurface.h \
    motorcar/src/scenegraph/output/wayland/waylanddrawable.h \
    motorcar/src/gl/openglshader.h \
    motorcar/src/gl/GLSLHelper.h \
    motorcar/src/gl/openglcontext.h \
    qt/src/qtwaylandmotorcaropenglcontext.h \
    motorcar/src/scenegraph/output/display/display.h \
    motorcar/src/device/oculushmdcontroller.h \
    motorcar/src/scenegraph/output/display/rendertotexturedisplay.h \
    motorcar/src/device/device.h






SOURCES += \
    src/main.cpp \
    qt/src/textureblitter.cpp \
    qt/src/qtwaylandmotorcarcompositor.cpp \
    qt/src/qopenglwindow.cpp \
    qt/src/opengldata.cpp \
    motorcar/src/geometry.cpp \
    motorcar/src/scenegraph/scenegraphnode.cpp \
    motorcar/src/scenegraph/output/glcameranode.cpp \
    motorcar/src/scenegraph/physicalnode.cpp \
    motorcar/src/scenegraph/virtualnode.cpp \
    motorcar/src/scenegraph/ioelement.cpp \
    motorcar/src/scenegraph/scene.cpp \
    motorcar/src/scenegraph/output/outputelement.cpp \
    motorcar/src/scenegraph/output/drawable.cpp \
    motorcar/src/scenegraph/output/wayland/waylandsurfacenode.cpp \
    motorcar/src/scenegraph/output/wayland/waylandsurface.cpp \
    qt/src/qtwaylandmotorcarsurface.cpp \
    motorcar/src/scenegraph/output/wayland/waylanddrawable.cpp \
    motorcar/src/gl/openglshader.cpp \
    motorcar/src/gl/GLSLHelper.cpp \
    motorcar/src/gl/openglcontext.cpp \
    qt/src/qtwaylandmotorcaropenglcontext.cpp \
    motorcar/src/scenegraph/output/display/display.cpp \
    motorcar/src/device/oculushmdcontroller.cpp \
    motorcar/src/scenegraph/output/display/rendertotexturedisplay.cpp







# to make QtCompositor/... style includes working without installing
INCLUDEPATH += $$PWD/../../include


#  if you want to compile QtCompositor as part of the application
#  instead of linking to it, remove the QT += compositor and uncomment
#  the following line
#include(../../src/compositor/compositor.pri)

RESOURCES += motorcar-compositor.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/qtwayland/motorcar-compositor
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS motorcar-compositor.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/qtwayland/motorcar-compositor
INSTALLS += target sources

OTHER_FILES += \
    src/motorcarsurface.vert \
    src/motorcarsurface.frag \
    src/motorcar/shaders/motorcarsurface.vert \
    src/motorcar/shaders/motorcarsurface.frag \
    motorcar/src/shaders/motorcarsurface.vert \
    motorcar/src/shaders/motorcarsurface.frag \
    motorcar/src/device/foo \
    motorcar/src/shaders/motorcarbarreldistortion.frag \
    motorcar/src/shaders/motorcarbarreldistortion.vert



