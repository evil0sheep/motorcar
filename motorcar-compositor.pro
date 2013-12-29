QT += gui gui-private core-private compositor

LIBS += -L ../../lib
#include (../../src/qt-compositor/qt-compositor.pri)

HEADERS += \
    qopenglwindow.h \
    motorcarcompositor.h \
    textureblitter.h \
    scenegraphnode.h \
    motorcarsurfacenode.h \
    opengldata.h

SOURCES += main.cpp \
    qopenglwindow.cpp \
    motorcarcompositor.cpp \
    textureblitter.cpp \
    scenegraphnode.cpp \
    motorcarsurfacenode.cpp \
    opengldata.cpp

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
