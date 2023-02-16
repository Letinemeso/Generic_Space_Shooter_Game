#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <Math_Stuff.h>
#include <Camera_2D.h>
#include <Object_System/Modules/Default_Draw_Module_2D.h>


namespace GSSG
{

    class Background
    {
    private:
        const LEti::Camera_2D* m_camera = nullptr;

    private:
        LEti::Default_Draw_Module_2D* m_draw_module = nullptr;
        const LEti::Picture* m_picture = nullptr;

        float m_screen_size_scale = 0.0f;

    public:
        Background();
        ~Background();

    public:
        inline void inject_camera(const LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void set_picture(const LEti::Picture* _picture) { m_picture = _picture; }

    private:
        void M_reconfigure();

    public:
        void update();

    public:
        inline const LEti::Default_Draw_Module_2D* draw_module() const { return m_draw_module; }

    };

}


#endif // BACKGROUND_H
