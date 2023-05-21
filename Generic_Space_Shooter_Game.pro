TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

unix {
    LIBS += "../L_Utility/libL_Utility.a"
    LIBS += "../L_Variable/libL_Variable.a"
    LIBS += "../L_Gamedev_Lib/libL_Gamedev_Lib.a"
    LIBS += "../LEti_Engine/libLEti_Engine.a"

    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libglfw3.a"
    LIBS += "../../LEti_Engine/OpenGL/Linux/lib/libGLEW.a"

    LIBS += -lpthread -lGL -lX11 -ldl

    INCLUDEPATH += "../LEti_Engine/OpenGL/Linux/include/"
}


win32 {
    LIBS += "../L_Utility/debug/libL_Utility.a"
    LIBS += "../L_Variable/debug/libL_Variable.a"
    LIBS += "../L_Gamedev_Lib/debug/libL_Gamedev_Lib.a"
    LIBS += "../LEti_Engine/debug/libLEti_Engine.a"

    LIBS += "../LEti_Engine/OpenGL/Windows_x64_mingw/lib/libglfw3.a"
    LIBS += "../LEti_Engine/OpenGL/Windows_x64_mingw/lib/libglew32.a"

    LIBS += -lopengl32 \
	-luser32 \
	-lgdi32 \
	-lshell32

    INCLUDEPATH += "../LEti_Engine/OpenGL/Windows_x64_mingw/include/"
}

INCLUDEPATH += "../LEti_Engine/OpenGL/GLM"
INCLUDEPATH += "../LEti_Engine/OpenGL/"

INCLUDEPATH += "include/"
INCLUDEPATH += "../L_Utility/include/"
INCLUDEPATH += "../L_Variable/include/"
INCLUDEPATH += "../L_Gamedev_Lib/include/"
INCLUDEPATH += "../LEti_Engine/include/"


SOURCES += \
    Main.cpp \
    source/Background.cpp \
    source/Collision_Resolution__Entity.cpp \
    source/Edit_Mode/Block_Controller.cpp \
    source/Edit_Mode/Edit_Mode.cpp \
    source/Edit_Mode/Grid.cpp \
    source/Effects_Controller.cpp \
    source/Enemy.cpp \
    source/Enemy_Generator.cpp \
    source/Entity.cpp \
    source/Entity_Manager.cpp \
    source/GUI.cpp \
    source/Game_Logic.cpp \
    source/Game_World.cpp \
    source/Player.cpp \
    source/Player_Controller.cpp \
    source/Projectile.cpp \
    source/Space_Ship.cpp

DISTFILES += \
Resources/Models/arrow_quad.mdl \
    Resources/Models/text_field_new.mdl \
Resources/Models/triangle.mdl \
Resources/Shaders/fragment_shader.shader \
Resources/Shaders/vertex_shader.shader \
Resources/Font/font_yellow.png \
Resources/Textures/textures.mdl \


HEADERS += \
        include/Background.h \
	include/Collision_Resolution__Entity.h \
	include/Edit_Mode/Block_Controller.h \
	include/Edit_Mode/Edit_Mode.h \
	include/Edit_Mode/Grid.h \
	include/Effects_Controller.h \
	include/Enemy.h \
	include/Enemy_Generator.h \
	include/Entity.h \
	include/Entity_Manager.h \
	include/GUI.h \
	include/Game_Logic.h \
	include/Game_World.h \
	include/Player.h \
	include/Player_Controller.h \
	include/Projectile.h \
	include/Space_Ship.h
