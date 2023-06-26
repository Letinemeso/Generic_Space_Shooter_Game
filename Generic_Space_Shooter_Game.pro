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


DISTFILES += \
Resources/Models/arrow_quad.mdl \
	Resources/Models/blocks.mdl \
	Resources/Models/explosion.mdl \
	Resources/Models/grid_cell.mdl \
	Resources/Models/projectile.mdl \
    Resources/Models/text_field_new.mdl \
Resources/Models/triangle.mdl \
Resources/Shaders/fragment_shader.shader \
Resources/Shaders/vertex_shader.shader \
Resources/Font/font_yellow.png \
	Resources/Textures/editing_mode_textures/_edit_mode_atlas.png \
	Resources/Textures/editing_mode_textures/edit_mode_atlas.png \
	Resources/Textures/editing_mode_textures/grid_11x11.png \
	Resources/Textures/editing_mode_textures/grid_cell.png \
	Resources/Textures/gameplay_textures/arrow.png \
	Resources/Textures/gameplay_textures/background.png \
	Resources/Textures/gameplay_textures/explosion_animation.png \
	Resources/Textures/gameplay_textures/projectile_animation.png \
	Resources/Textures/gameplay_textures/triangle.png \
	Resources/Textures/gameplay_textures/white.png \
	Resources/Textures/gameplay_textures/yellow.png \
Resources/Textures/textures.mdl \

HEADERS += \
	include/Edit_Mode/Block_Controller.h \
	include/Edit_Mode/Edit_Mode.h \
	include/Edit_Mode/Grid.h \
	include/Effects_Controller.h \
	include/GUI.h \
	include/Game_Logic.h \
	include/Game_World/Background.h \
	include/Game_World/Collision_Resolution__Entity.h \
	include/Game_World/Enemy.h \
	include/Game_World/Enemy_Generator.h \
	include/Game_World/Entity.h \
	include/Game_World/Entity_Manager.h \
	include/Game_World/Game_World.h \
	include/Game_World/Player.h \
	include/Game_World/Player_Controller.h \
	include/Game_World/Projectile.h \
	include/Game_World/Space_Ship.h

SOURCES += \
	Main.cpp \
	source/Edit_Mode/Block_Controller.cpp \
	source/Edit_Mode/Edit_Mode.cpp \
	source/Edit_Mode/Grid.cpp \
	source/Effects_Controller.cpp \
	source/GUI.cpp \
	source/Game_Logic.cpp \
	source/Game_World/Background.cpp \
	source/Game_World/Collision_Resolution__Entity.cpp \
	source/Game_World/Enemy.cpp \
	source/Game_World/Enemy_Generator.cpp \
	source/Game_World/Entity.cpp \
	source/Game_World/Entity_Manager.cpp \
	source/Game_World/Game_World.cpp \
	source/Game_World/Player.cpp \
	source/Game_World/Player_Controller.cpp \
	source/Game_World/Projectile.cpp \
	source/Game_World/Space_Ship.cpp


