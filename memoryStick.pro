######################################################################
# Automatically generated by qmake (3.0) Thu Jan 26 16:35:29 2017
######################################################################

include(../common/common.pri)
TARGET = phong

# Input
HEADERS += \
           ../common/include/camera.h \
           ../common/include/fixedcamera.h \
           ../common/include/scene.h \
           ../common/include/trackballcamera.h \
           src/shaderscene.h
SOURCES += src/main.cpp \
           ../common/src/camera.cpp \
           ../common/src/fixedcamera.cpp \
           ../common/src/scene.cpp \
           ../common/src/trackballcamera.cpp \
           src/shaderscene.cpp

OTHER_FILES += shaders/plastic_vert.glsl \
               shaders/plastic_frag.glsl \
               shaders/metal_vert.glsl \
               shaders/metal_frag.glsl \
               shaders/bump_vert.glsl \
               shaders/bump_frag.glsl \
               shaders/env_vert.glsl \
               shaders/env_frag.glsl \
               models/memoryStickPlastic.obj \
               models/memoryStickMetal.obj \
               models/memoryStickAdaptor.obj \
               models/memoryStickFloor.obj \
               images/cubeX.png




