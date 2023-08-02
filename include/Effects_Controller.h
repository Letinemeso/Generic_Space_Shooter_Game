#ifndef EFFECTS_CONTROLLER_H
#define EFFECTS_CONTROLLER_H

#include <Data_Structures/List.h>

#include <Renderer/Renderer.h>
#include <Object_System/Object_2D.h>
#include <Draw_Modules/Draw_Module__Animation.h>


namespace GSSG
{

    class Effects_Controller
    {
    private:
        const LR::Renderer* m_renderer = nullptr;

    private:
        LDS::List<LEti::Object_2D*> m_effect_objects;

    public:
        Effects_Controller();
        Effects_Controller(const Effects_Controller& _other) = delete;
        Effects_Controller(Effects_Controller&& _other) = delete;
        ~Effects_Controller();

    public:
        inline void inject_renderer(LR::Renderer* _renderer) { m_renderer = _renderer; }

    public:
        void add_object(LEti::Object_2D* _object);

    public:
        void update();
        void draw();

    };

}

#endif // EFFECTS_CONTROLLER_H
