QT += gui widgets gui-private core-private compositor compositor-private

TEMPLATE = lib


#QTWAYLANDSOURCEPATH=$$PWD/../thirdPartySource/qt5_GLES/qtwayland/


#LIBOVRPATH=$$PWD/../thirdPartySource/Oculus/OculusSDK/LibOVR
#SIXENSEPATH=$$PWD/../thirdPartySource/sixenseSDK_linux_OSX
#RELEASETYPE=Release
#SYSARCH=x86_64
#INCLUDEPATH += "$$LIBOVRPATH/Include" "$$LIBOVRPATH/Src" "$$SIXENSEPATH/include" /opt/softkinetic/DepthSenseSDK/include
#LIBS += -L/home/dave/thesis/thirdPartySource/sixenseSDK_linux_OSX/lib/linux_x64/release -lsixense_utils_x64 -lsixense_x64
#LIBS += -L/opt/softkinetic/DepthSenseSDK/lib/ -lDepthSense
#LIBS += -L$$LIBOVRPATH/Lib/Linux/$$RELEASETYPE/$$SYSARCH -lovr
#LIBS +=  -ludev -lpthread -lGL -lX11 -lXinerama
#LIBS +=  -lwayland-server -lwayland-client


MOTORCAR_PROTOCOL_PATH=$$PWD/src/protocol/
system(cd $$MOTORCAR_PROTOCOL_PATH; make)


libmotorcarclient.commands = echo "testing123" && cd src/client && make -f Makefile

QMAKE_EXTRA_TARGETS += libmotorcarclient

INCLUDEPATH += $$MOTORCAR_PROTOCOL_PATH
#LIBS +=  -L $$MOTORCAR_PROTOCOL_PATH -lmotorcar-server
HEADERS += $$MOTORCAR_PROTOCOL_PATH/motorcar-server-protocol.h
SOURCES += $$MOTORCAR_PROTOCOL_PATH/motorcar-wayland-extensions.c

DESTDIR = lib
OBJECTS_DIR = lib/.obj
MOC_DIR = lib/.moc
RCC_DIR = lib/.rcc
UI_DIR = lib/.ui

QMAKE_CXXFLAGS += -std=c++11 -DGL_GLEXT_PROTOTYPES -DMOTORCAR_SHADER_PATH=$$PWD/src/compositor/shaders


LIBS += -lGL
LIBS += -L $$QTWAYLANDSOURCEPATH/lib
INCLUDEPATH += $$QTWAYLANDSOURCEPATH/include
#INCLUDEPATH += $$QTWAYLANDSOURCEPATH/include/QtCompositor/5.3.0/

#  if you want to compile QtCompositor as part of the application
#  instead of linking to it, remove the QT += compositor and uncomment
#  the following line
#include($$PWD/../thirdPartySource/qt5_GLES/qtwayland/src/compositor/compositor.pri)

#RESOURCES += motorcar-compositor.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/qtwayland/motorcar-compositor
target.depends = libmotorcarclient
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS motorcar-compositor.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/qtwayland/motorcar-compositor
INSTALLS += target sources

INCLUDEPATH += src/compositor

HEADERS += \
    src/compositor/qt/textureblitter.h \
    src/compositor/qt/qtwaylandmotorcarcompositor.h \
    src/compositor/qt/qopenglwindow.h \
    src/compositor/qt/opengldata.h \
    src/compositor/geometry.h \
    src/compositor/scenegraph/scenegraphnode.h \
    src/compositor/scenegraph/scenegraph.h \
    src/compositor/scenegraph/physicalnode.h \
    src/compositor/scenegraph/virtualnode.h \
    src/compositor/scenegraph/scene.h \
    src/compositor/scenegraph/output/drawable.h \
    src/compositor/scenegraph/output/wayland/waylandsurfacenode.h \
    src/compositor/scenegraph/output/output.h \
    src/compositor/motorcar.h \
    src/compositor/qt/qtwaylandmotorcarsurface.h \
    src/compositor/gl/openglshader.h \
    src/compositor/gl/GLSLHelper.h \
    src/compositor/gl/openglcontext.h \
    src/compositor/qt/qtwaylandmotorcaropenglcontext.h \
    src/compositor/scenegraph/output/display/display.h \
    src/compositor/scenegraph/output/display/rendertotexturedisplay.h \
    src/compositor/scenegraph/output/wireframenode.h \
    src/compositor/compositor.h \
    src/compositor/scenegraph/input/bone.h \
    src/compositor/scenegraph/input/bonesensor.h \
    src/compositor/scenegraph/input/skeleton.h \
    src/compositor/scenegraph/input/singlebonetracker.h \
    src/compositor/displayserver.h \
    src/compositor/windowmanager.h \
    src/compositor/events/event.h \
    src/compositor/events/mouseevent.h \
    src/compositor/events/events.h \
    src/compositor/events/keyboardevent.h \
    src/compositor/wayland/input/seat.h \
    src/compositor/wayland/input/pointer.h \
    src/compositor/wayland/input/keyboard.h \
    src/compositor/wayland/input/waylandinput.h \
    src/compositor/wayland/output/waylandsurface.h \
    src/compositor/qt/qtwaylandmotorcarseat.h \
    src/compositor/scenegraph/input/input.h \
    src/compositor/shell.h \
    src/compositor/scenegraph/output/viewpoint.h \
    src/compositor/gl/viewport.h \
    src/compositor/events/sixdofevent.h \
    src/compositor/scenegraph/input/sixdofpointingdevice.h \
    src/compositor/scenegraph/output/wayland/motorcarsurfacenode.h\
#    src/device/device.h \
#    src/device/oculushmd.h \
#    src/device/sixensemotionsensingsystem.h \
#    src/device/sixensecontrollernode.h \
#    src/device/sixensebasenode.h \
#    src/device/softkineticdepthcamera.h


SOURCES += \
    src/compositor/qt/textureblitter.cpp \
    src/compositor/qt/qtwaylandmotorcarcompositor.cpp \
    src/compositor/qt/qopenglwindow.cpp \
    src/compositor/qt/opengldata.cpp \
    src/compositor/geometry.cpp \
    src/compositor/scenegraph/scenegraphnode.cpp \
    src/compositor/scenegraph/physicalnode.cpp \
    src/compositor/scenegraph/virtualnode.cpp \
    src/compositor/scenegraph/scene.cpp \
    src/compositor/scenegraph/output/drawable.cpp \
    src/compositor/scenegraph/output/wayland/waylandsurfacenode.cpp \
    src/compositor/qt/qtwaylandmotorcarsurface.cpp \
    src/compositor/gl/openglshader.cpp \
    src/compositor/gl/GLSLHelper.cpp \
    src/compositor/gl/openglcontext.cpp \
    src/compositor/qt/qtwaylandmotorcaropenglcontext.cpp \
    src/compositor/scenegraph/output/display/display.cpp \
    src/compositor/scenegraph/output/display/rendertotexturedisplay.cpp \
    src/compositor/scenegraph/output/wireframenode.cpp \
    src/compositor/compositor.cpp \
    src/compositor/scenegraph/input/bone.cpp \
    src/compositor/scenegraph/input/bonesensor.cpp \
    src/compositor/scenegraph/input/skeleton.cpp \
    src/compositor/scenegraph/input/singlebonetracker.cpp \
    src/compositor/displayserver.cpp \
    src/compositor/windowmanager.cpp \
    src/compositor/events/event.cpp \
    src/compositor/events/mouseevent.cpp \
    src/compositor/events/keyboardevent.cpp \
    src/compositor/wayland/input/seat.cpp \
    src/compositor/wayland/input/pointer.cpp \
    src/compositor/wayland/input/keyboard.cpp \
    src/compositor/wayland/output/waylandsurface.cpp \
    src/compositor/qt/qtwaylandmotorcarseat.cpp \
    src/compositor/shell.cpp \
    src/compositor/scenegraph/output/viewpoint.cpp \
    src/compositor/gl/viewport.cpp \
    src/compositor/events/sixdofevent.cpp \
    src/compositor/scenegraph/input/sixdofpointingdevice.cpp \
    src/compositor/scenegraph/output/wayland/motorcarsurfacenode.cpp\
#    src/device/oculushmd.cpp \
#    src/device/sixensemotionsensingsystem.cpp \
#    src/device/sixensecontrollernode.cpp \
#    src/device/sixensebasenode.cpp \
#    src/device/softkineticdepthcamera.cpp\
#    src/examples/compositors/rift-hydra-compositor/main.cpp



OTHER_FILES += \
    src/motorcarsurface.vert \
    src/motorcarsurface.frag \
    src/motorcar/shaders/motorcarsurface.vert \
    src/motorcar/shaders/motorcarsurface.frag \
    src/compositor/shaders/motorcarsurface.vert \
    src/compositor/shaders/motorcarsurface.frag \
    src/compositor/shaders/motorcarbarreldistortion.frag \
    src/compositor/shaders/motorcarbarreldistortion.vert \
    src/compositor/shaders/motorcarline.vert \
    src/compositor/shaders/motorcarline.frag \
    src/compositor/shaders/depthcompositedsurface.vert \
    src/compositor/shaders/depthcompositedsurface.frag \
    src/compositor/shaders/depthcompositedsurfaceblitter.frag \
    src/compositor/shaders/depthcompositedsurfaceblitter.vert \
    src/compositor/shaders/softkineticdepthcam.vert \
    src/compositor/shaders/softkineticdepthcam.frag



