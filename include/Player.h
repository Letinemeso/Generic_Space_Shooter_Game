#ifndef PLAYER_H
#define PLAYER_H


#include <Data_Structures/List.h>

#include <Event_Controller.h>

#include <Enemy_Entity_Generator.h>
#include <Entity_Manager.h>
#include <Entity.h>
#include <Projectile.h>


namespace GSSG
{

    class Player : public GSSG::Entity
    {
    private:
        LEti::Camera_2D* m_camera = nullptr;

    private:
        Entity_Manager* m_entity_manager = nullptr;
        Enemy_Entity_Generator* m_enemy_generator = nullptr;

    private:
        const LEti::Object_2D_Stub* m_projectile_stub = nullptr;

        float m_acceleration_speed = 300.0f;
        float m_rotation_acceleration_speed = LEti::Math::HALF_PI;
        float m_max_multiplier_limit = 1.0f;

    public:
        Player();
        ~Player();

    public:
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }
        inline void inject_enemy_generator(Enemy_Entity_Generator* _enemy_generator) { m_enemy_generator = _enemy_generator; }

        inline void set_projectile_stub(const LEti::Object_2D_Stub* _projectile_stub) { m_projectile_stub = _projectile_stub; }

    public:
        void apply_input();
        void update(float _ratio = 1.0f) override;

    };

}


#endif // PLAYER_H
