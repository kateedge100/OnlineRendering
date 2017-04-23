#############################################################################
# Makefile for building: memoryStick
# Generated by qmake (3.0) (Qt 5.5.0)
# Project:  memoryStick.pro
# Template: app
# Command: /usr/bin/qmake-qt5 -spec linux-g++ CONFIG+=debug -o Makefile memoryStick.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT5BUILD -DNGL_DEBUG -DQT_OPENGL_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -msse -msse2 -msse3 -march=native -march=native -g -std=c++0x -Wall -W -Wno-unused-parameter -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I. -I. -isystem /usr/include -I/home/kedge/NGL/include -I../common/include -I/public/devel/libigl/include -isystem /usr/local/include/eigen3 -I/public/devel/include/noise -I/usr/local/lib -I/home/kedge/NGL/include -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtOpenGL -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I/usr/lib64/qt5/mkspecs/linux-g++
QMAKE         = /usr/bin/qmake-qt5
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = memoryStick1.0.0
DISTDIR = /home/kedge/Desktop/Rendering/Realtime\ Assignment/OnlineRendering/obj/memoryStick1.0.0
LINK          = g++
LFLAGS        = -Wl,-rpath,/home/kedge/NGL/lib -Wl,-rpath,/home/kedge/Desktop/Rendering/Realtime\ Assignment/OnlineRendering/L/home/kedge/NGL/lib -Wl,-rpath,/home/kedge/NGL/lib
LIBS          = $(SUBLIBS) -lXi -ldl -lX11 -lXrandr -lXinerama -lXcursor -lXxf86vm -lglfw3 -lGLEW -lGLU -lNGL -L/home/kedge/NGL/lib -L/usr/local/lib -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = 

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = src/main.cpp \
		../common/src/camera.cpp \
		../common/src/fixedcamera.cpp \
		../common/src/scene.cpp \
		../common/src/trackballcamera.cpp \
		src/shaderscene.cpp 
OBJECTS       = obj/main.o \
		obj/camera.o \
		obj/fixedcamera.o \
		obj/scene.o \
		obj/trackballcamera.o \
		obj/shaderscene.o
DIST          = shaders/shadow_frag.glsl \
		shaders/shadow_vert.glsl \
		.qmake.stash \
		memoryStick.pro ../common/include/camera.h \
		../common/include/fixedcamera.h \
		../common/include/scene.h \
		../common/include/trackballcamera.h \
		src/shaderscene.h src/main.cpp \
		../common/src/camera.cpp \
		../common/src/fixedcamera.cpp \
		../common/src/scene.cpp \
		../common/src/trackballcamera.cpp \
		src/shaderscene.cpp
QMAKE_TARGET  = memoryStick
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = memoryStick


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: memoryStick.pro .qmake.cache /usr/lib64/qt5/mkspecs/linux-g++/qmake.conf /usr/lib64/qt5/mkspecs/features/spec_pre.prf \
		/usr/lib64/qt5/mkspecs/common/unix.conf \
		/usr/lib64/qt5/mkspecs/common/linux.conf \
		/usr/lib64/qt5/mkspecs/common/sanitize.conf \
		/usr/lib64/qt5/mkspecs/common/gcc-base.conf \
		/usr/lib64/qt5/mkspecs/common/gcc-base-unix.conf \
		/usr/lib64/qt5/mkspecs/common/g++-base.conf \
		/usr/lib64/qt5/mkspecs/common/g++-unix.conf \
		/usr/lib64/qt5/mkspecs/qconfig.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_concurrent.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_concurrent_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_core.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_core_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_dbus.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_dbus_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_gui.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_gui_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_network.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_network_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_opengl.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_opengl_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_openglextensions.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_printsupport.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_printsupport_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_sql.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_sql_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_testlib.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_testlib_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_widgets.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_widgets_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_xml.pri \
		/usr/lib64/qt5/mkspecs/modules/qt_lib_xml_private.pri \
		/usr/lib64/qt5/mkspecs/features/qt_functions.prf \
		/usr/lib64/qt5/mkspecs/features/qt_config.prf \
		/usr/lib64/qt5/mkspecs/linux-g++/qmake.conf \
		/usr/lib64/qt5/mkspecs/features/spec_post.prf \
		.qmake.cache \
		.qmake.stash \
		/usr/lib64/qt5/mkspecs/features/exclusive_builds.prf \
		/usr/lib64/qt5/mkspecs/features/default_pre.prf \
		/home/kedge/NGL/UseNGL.pri \
		/home/kedge/Desktop/Rendering/Realtime\ Assignment/common/common.pri \
		/usr/lib64/qt5/mkspecs/features/resolve_config.prf \
		/usr/lib64/qt5/mkspecs/features/default_post.prf \
		/usr/lib64/qt5/mkspecs/features/c++11.prf \
		/usr/lib64/qt5/mkspecs/features/warn_on.prf \
		/usr/lib64/qt5/mkspecs/features/qt.prf \
		/usr/lib64/qt5/mkspecs/features/resources.prf \
		/usr/lib64/qt5/mkspecs/features/moc.prf \
		/usr/lib64/qt5/mkspecs/features/unix/opengl.prf \
		/usr/lib64/qt5/mkspecs/features/uic.prf \
		/usr/lib64/qt5/mkspecs/features/unix/thread.prf \
		/usr/lib64/qt5/mkspecs/features/testcase_targets.prf \
		/usr/lib64/qt5/mkspecs/features/exceptions.prf \
		/usr/lib64/qt5/mkspecs/features/yacc.prf \
		/usr/lib64/qt5/mkspecs/features/lex.prf \
		memoryStick.pro \
		/usr/lib64/libQt5OpenGL.prl \
		/usr/lib64/libQt5Widgets.prl \
		/usr/lib64/libQt5Gui.prl \
		/usr/lib64/libQt5Core.prl
	$(QMAKE) -spec linux-g++ CONFIG+=debug -o Makefile memoryStick.pro
/usr/lib64/qt5/mkspecs/features/spec_pre.prf:
/usr/lib64/qt5/mkspecs/common/unix.conf:
/usr/lib64/qt5/mkspecs/common/linux.conf:
/usr/lib64/qt5/mkspecs/common/sanitize.conf:
/usr/lib64/qt5/mkspecs/common/gcc-base.conf:
/usr/lib64/qt5/mkspecs/common/gcc-base-unix.conf:
/usr/lib64/qt5/mkspecs/common/g++-base.conf:
/usr/lib64/qt5/mkspecs/common/g++-unix.conf:
/usr/lib64/qt5/mkspecs/qconfig.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_bootstrap_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_concurrent.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_concurrent_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_core.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_core_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_dbus.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_dbus_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_eglfs_device_lib_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_gui.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_gui_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_network.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_network_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_opengl.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_opengl_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_openglextensions.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_openglextensions_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_platformsupport_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_printsupport.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_printsupport_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_sql.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_sql_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_testlib.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_testlib_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_widgets.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_widgets_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_xml.pri:
/usr/lib64/qt5/mkspecs/modules/qt_lib_xml_private.pri:
/usr/lib64/qt5/mkspecs/features/qt_functions.prf:
/usr/lib64/qt5/mkspecs/features/qt_config.prf:
/usr/lib64/qt5/mkspecs/linux-g++/qmake.conf:
/usr/lib64/qt5/mkspecs/features/spec_post.prf:
.qmake.cache:
.qmake.stash:
/usr/lib64/qt5/mkspecs/features/exclusive_builds.prf:
/usr/lib64/qt5/mkspecs/features/default_pre.prf:
/home/kedge/NGL/UseNGL.pri:
/home/kedge/Desktop/Rendering/Realtime\ Assignment/common/common.pri:
/usr/lib64/qt5/mkspecs/features/resolve_config.prf:
/usr/lib64/qt5/mkspecs/features/default_post.prf:
/usr/lib64/qt5/mkspecs/features/c++11.prf:
/usr/lib64/qt5/mkspecs/features/warn_on.prf:
/usr/lib64/qt5/mkspecs/features/qt.prf:
/usr/lib64/qt5/mkspecs/features/resources.prf:
/usr/lib64/qt5/mkspecs/features/moc.prf:
/usr/lib64/qt5/mkspecs/features/unix/opengl.prf:
/usr/lib64/qt5/mkspecs/features/uic.prf:
/usr/lib64/qt5/mkspecs/features/unix/thread.prf:
/usr/lib64/qt5/mkspecs/features/testcase_targets.prf:
/usr/lib64/qt5/mkspecs/features/exceptions.prf:
/usr/lib64/qt5/mkspecs/features/yacc.prf:
/usr/lib64/qt5/mkspecs/features/lex.prf:
memoryStick.pro:
/usr/lib64/libQt5OpenGL.prl:
/usr/lib64/libQt5Widgets.prl:
/usr/lib64/libQt5Gui.prl:
/usr/lib64/libQt5Core.prl:
qmake: FORCE
	@$(QMAKE) -spec linux-g++ CONFIG+=debug -o Makefile memoryStick.pro

qmake_all: FORCE


all: Makefile $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/
	$(COPY_FILE) --parents ../common/include/camera.h ../common/include/fixedcamera.h ../common/include/scene.h ../common/include/trackballcamera.h src/shaderscene.h $(DISTDIR)/
	$(COPY_FILE) --parents src/main.cpp ../common/src/camera.cpp ../common/src/fixedcamera.cpp ../common/src/scene.cpp ../common/src/trackballcamera.cpp src/shaderscene.cpp $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) .qmake.stash
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

obj/main.o: src/main.cpp src/shaderscene.h \
		../common/include/scene.h \
		../common/include/glinclude.h \
		/home/kedge/NGL/include/ngl/Obj.h \
		/home/kedge/NGL/include/ngl/Types.h \
		/home/kedge/NGL/include/ngl/glew.h \
		/home/kedge/NGL/include/ngl/Texture.h \
		/home/kedge/NGL/include/ngl/Image.h \
		/home/kedge/NGL/include/ngl/Colour.h \
		/home/kedge/NGL/include/ngl/Vec4.h \
		/home/kedge/NGL/include/ngl/Vec2.h \
		/home/kedge/NGL/include/ngl/Vec3.h \
		/home/kedge/NGL/include/ngl/AbstractMesh.h \
		/home/kedge/NGL/include/ngl/BBox.h \
		/home/kedge/NGL/include/ngl/AbstractVAO.h \
		/home/kedge/NGL/include/ngl/RibExport.h \
		/home/kedge/NGL/include/ngl/NGLassert.h \
		../common/include/fixedcamera.h \
		/home/kedge/Desktop/Rendering/Realtime\ Assignment/common/include/camera.h \
		../common/include/trackballcamera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o src/main.cpp

obj/camera.o: ../common/src/camera.cpp /home/kedge/Desktop/Rendering/Realtime\ Assignment/common/include/camera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/camera.o ../common/src/camera.cpp

obj/fixedcamera.o: ../common/src/fixedcamera.cpp ../common/include/fixedcamera.h \
		/home/kedge/Desktop/Rendering/Realtime\ Assignment/common/include/camera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/fixedcamera.o ../common/src/fixedcamera.cpp

obj/scene.o: ../common/src/scene.cpp ../common/include/scene.h \
		../common/include/glinclude.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/scene.o ../common/src/scene.cpp

obj/trackballcamera.o: ../common/src/trackballcamera.cpp ../common/include/trackballcamera.h \
		/home/kedge/Desktop/Rendering/Realtime\ Assignment/common/include/camera.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/trackballcamera.o ../common/src/trackballcamera.cpp

obj/shaderscene.o: src/shaderscene.cpp src/shaderscene.h \
		../common/include/scene.h \
		../common/include/glinclude.h \
		/home/kedge/NGL/include/ngl/Obj.h \
		/home/kedge/NGL/include/ngl/Types.h \
		/home/kedge/NGL/include/ngl/glew.h \
		/home/kedge/NGL/include/ngl/Texture.h \
		/home/kedge/NGL/include/ngl/Image.h \
		/home/kedge/NGL/include/ngl/Colour.h \
		/home/kedge/NGL/include/ngl/Vec4.h \
		/home/kedge/NGL/include/ngl/Vec2.h \
		/home/kedge/NGL/include/ngl/Vec3.h \
		/home/kedge/NGL/include/ngl/AbstractMesh.h \
		/home/kedge/NGL/include/ngl/BBox.h \
		/home/kedge/NGL/include/ngl/AbstractVAO.h \
		/home/kedge/NGL/include/ngl/RibExport.h \
		/home/kedge/NGL/include/ngl/NGLassert.h \
		/home/kedge/NGL/include/ngl/NGLInit.h \
		/home/kedge/NGL/include/ngl/Singleton.h \
		/home/kedge/NGL/include/ngl/VAOPrimitives.h \
		/home/kedge/NGL/include/ngl/ShaderLib.h \
		/home/kedge/NGL/include/ngl/Shader.h \
		/home/kedge/NGL/include/ngl/ShaderProgram.h \
		/home/kedge/NGL/include/ngl/Util.h \
		/home/kedge/NGL/include/ngl/Mat3.h \
		/home/kedge/NGL/include/ngl/Mat4.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/shaderscene.o src/shaderscene.cpp

####### Install

install:  FORCE

uninstall:  FORCE

FORCE:

