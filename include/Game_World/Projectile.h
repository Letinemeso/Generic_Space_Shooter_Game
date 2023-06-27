#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Physics/Collision_Detector_2D.h>

#include <Game_World/Entity.h>


namespace GSSG
{

    class Projectile : public Entity
    {
    public:
        DECLARE_VARIABLE;

    private:
        float m_time_until_death_prev_state = 0.0f;
        float m_time_until_death = 0.0f;
        bool m_has_limited_lifetime = false;

    public:
        Projectile();
        ~Projectile();

    public:
        void revert_to_previous_state() override;
        void update_previous_state() override;
        void update(float _ratio = 1.0f) override;

        void on_collision(const LEti::Intersection_Data& _id) override;

        void apply_input() override { }

    public:
        void set_lifetime(float _seconds);

    public:
        bool should_be_destroyed() const override;

    };

    class Projectile_Stub : public Entity_Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LV::Variable_Base* M_construct_product() const override { return new Projectile; }

    };

}


#endif // PROJECTILE_H
