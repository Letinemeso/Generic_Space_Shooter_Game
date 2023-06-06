#ifndef ENEMY_GENERATOR_H
#define ENEMY_GENERATOR_H

#include <Math_Stuff.h>
#include <Stuff/Timer.h>

#include <Game_World/Entity_Manager.h>
#include <Game_World/Enemy.h>
#include <Effects_Controller.h>


namespace GSSG
{

    class Enemy_Generator
    {
    private:
        const LEti::Camera_2D* m_camera = nullptr;

    private:
        Entity_Manager* m_entity_manager = nullptr;
        const LEti::Object_2D_Stub* m_enemy_stub = nullptr;
        const LEti::Object_2D_Stub* m_enemy_projectile_stub = nullptr;
        Effects_Controller* m_effects_controller = nullptr;

        float m_spawn_frequency = 1.0f;
        LST::Timer m_timer;

    public:
        Enemy_Generator();
        ~Enemy_Generator();

    public:
        inline void set_enemy_stub(const LEti::Object_2D_Stub* _enemy_stub) { m_enemy_stub = _enemy_stub; }
        inline void set_enemy_projectile_stub(const LEti::Object_2D_Stub* _enemy_projectile_stub) { m_enemy_projectile_stub = _enemy_projectile_stub; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }
        inline void inject_camera(const LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_effects_controller(Effects_Controller* _effects_controller) { m_effects_controller = _effects_controller; }
        inline void set_spawn_frequency(float _spawn_frequency) { m_spawn_frequency = _spawn_frequency; }

    public:
        void update();

    public:
        void spawn_enemy() const;

    };

}


#endif // ENEMY_GENERATOR_H
