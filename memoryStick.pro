######################################################################
# Automatically generated by qmake (3.0) Thu Dec 1 11:41:32 2016
######################################################################

include(../common/common.pri)
TARGET = memoryStick

# Input
SOURCES += src/main.cpp \
           ../common/src/scene.cpp \
           ../common/src/camera.cpp \
           ../common/src/trackballcamera.cpp \
    src/shaderScene.cpp

HEADERS += \
           ../common/include/scene.h \
           ../common/include/camera.h \
           ../common/include/trackballcamera.h \
    src/shaderScene.h

OTHER_FILES += shaders/*.glsl

DISTFILES += \
	     shaders/dof_frag.glsl \
             shaders/dof_vert.glsl \
             shaders/plastic_frag.glsl \
             shaders/plastic_vert.glsl \
               shaders/metal_vert.glsl \
               shaders/metal_frag.glsl \
               shaders/floor_vert.glsl \
               shaders/floor_frag.glsl \
               shaders/shadow_vert.glsl \
               shaders/shadow_frag.glsl \
             model/memoryStickPlastic.obj \
             images/env_top.png \

	
	
