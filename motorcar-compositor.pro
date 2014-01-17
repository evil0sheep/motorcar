QT += gui gui-private core-private compositor


DESTDIR = bin
OBJECTS_DIR = bin/.obj
MOC_DIR = bin/.moc
RCC_DIR = bin/.rcc
UI_DIR = bin/.ui



LIBS += -L ../../lib
#include (../../src/qt-compositor/qt-compositor.pri)
#include(scenegraph/include.pri)

HEADERS += \
    qt/src/textureblitter.h \
    qt/src/qtwaylandsurfacenode.h \
    qt/src/qtwaylandmotorcarcompositor.h \
    qt/src/qopenglwindow.h \
    qt/src/opengldata.h \
    qt/src/defaultdisplaynode.h \
    motorcar/src/geometry.h \
    motorcar/src/scenegraph/scenegraphnode_old.h \
    motorcar/src/scenegraph/scenegraphnode.h \
    motorcar/src/scenegraph/scenegraph.h \
    motorcar/src/scenegraph/output/glcameranode.h \
    motorcar/src/scenegraph/output/displaynode.h






SOURCES += \
    src/main.cpp \
    qt/src/textureblitter.cpp \
    qt/src/qtwaylandsurfacenode.cpp \
    qt/src/qtwaylandmotorcarcompositor.cpp \
    qt/src/qopenglwindow.cpp \
    qt/src/opengldata.cpp \
    qt/src/defaultdisplaynode.cpp \
    motorcar/src/geometry.cpp \
    motorcar/src/scenegraph/scenegraphnode_old.cpp \
    motorcar/src/scenegraph/scenegraphnode.cpp \
    motorcar/src/scenegraph/output/glcameranode.cpp \
    motorcar/src/scenegraph/output/displaynode.cpp






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
    motorcar/src/shaders/motorcarsurface.frag



