#ifndef EFFECTS_CONTROLLER_H
#define EFFECTS_CONTROLLER_H

#include <Data_Structures/List.h>

#include <Renderer/Renderer.h>

#include <Game_World/Visual_Effect.h>


namespace GSSG
{

    class Effects_Controller
    {
    private:
        const LR::Renderer* m_renderer = nullptr;

    private:
        LDS::List<Visual_Effect*> m_effect_objects;

    public:
        Effects_Controller();
        Effects_Controller(const Effects_Controller& _other) = delete;
        Effects_Controller(Effects_Controller&& _other) = delete;
        ~Effects_Controller();

    public:
        inline void inject_renderer(LR::Renderer* _renderer) { m_renderer = _renderer; }

    public:
        void add_object(Visual_Effect* _object);

    public:
        void update(float _dt);

    };

}

#endif // EFFECTS_CONTROLLER_H
