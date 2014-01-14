QT += gui gui-private core-private compositor

LIBS += -L ../../lib
#include (../../src/qt-compositor/qt-compositor.pri)
#include(scenegraph/include.pri)

HEADERS += \
    src/textureblitter.h \
    src/qopenglwindow.h \
    src/opengldata.h \
    src/motorcarcompositor.h \
    src/geometry.h \
    src/scenegraph/scenegraphnode.h \
    src/scenegraph/scenegraph.h \
    src/scenegraph/qtwaylandsurfacenode.h \
    src/scenegraph/glcameranode.h \
    src/scenegraph/displaynode.h \
    src/scenegraph/defaultdisplaynode.h

SOURCES += \
    src/textureblitter.cpp \
    src/qopenglwindow.cpp \
    src/opengldata.cpp \
    src/motorcarcompositor.cpp \
    src/main.cpp \
    src/geometry.cpp \
    src/scenegraph/scenegraphnode.cpp \
    src/scenegraph/qtwaylandsurfacenode.cpp \
    src/scenegraph/glcameranode.cpp \
    src/scenegraph/displaynode.cpp \
    src/scenegraph/defaultdisplaynode.cpp

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
    src/motorcarsurface.frag



