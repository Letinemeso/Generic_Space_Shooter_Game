#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <Game_World/Entity_Manager.h>
#include <Game_World/Entity.h>
#include <Game_World/Projectile.h>
#include <Game_World/Space_Ship_Structure.h>


namespace GSSG
{

    class Space_Ship : public Entity
    {
    public:
        DECLARE_VARIABLE;

    protected:
        Entity_Manager* m_entity_manager = nullptr;

    private:
        const LEti::Object_2D_Stub* m_projectile_stub = nullptr;

        float m_acceleration = 500.0f;
        float m_max_speed = 250.0f;
        float m_rotation_acceleration = LEti::Math::DOUBLE_PI;
        float m_max_rotation_speed = LEti::Math::PI;

    public:
        Space_Ship();
        ~Space_Ship();

    public:
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }

        inline void set_projectile_stub(const LEti::Object_2D_Stub* _projectile_stub) { m_projectile_stub = _projectile_stub; }

    public:
        inline float acceleration() const { return m_acceleration; }
        inline float max_speed() const { return m_max_speed; }
        inline float rotation_acceleration() const { return m_rotation_acceleration; }
        inline float max_rotation_speed() const { return m_max_rotation_speed; }

    protected:
        void M_shoot();

    public:
//        bool should_be_destroyed() const override;

    public:
        void update() override;

    };

}


#endif // SPACE_SHIP_H
