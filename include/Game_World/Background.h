#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <Transformation_Data.h>

#include <Math_Stuff.h>
#include <Camera/Camera_2D.h>
#include <Draw_Modules/Default_Draw_Module_2D.h>


namespace GSSG
{

    class Background
    {
    private:
        const LR::Camera_2D* m_camera = nullptr;
        LR::Renderer* m_renderer = nullptr;

    private:
        LR::Default_Draw_Module_2D* m_draw_module = nullptr;
        const LR::Picture* m_picture = nullptr;

        float m_screen_size_scale = 0.0f;

        LEti::Transformation_Data m_transformation_data;

    public:
        Background();
        ~Background();

    public:
        inline void inject_camera(const LR::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_renderer(LR::Renderer* _renderer) { m_renderer = _renderer; }
        inline void set_picture(const LR::Picture* _picture) { m_picture = _picture; }

    private:
        void M_reconfigure();

    public:
        void update(float _dt);

    public:
        inline const LR::Default_Draw_Module_2D* draw_module() const { return m_draw_module; }

    };

}


#endif // BACKGROUND_H
