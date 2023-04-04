#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Timer.h>
#include <Physics/Collision_Detector_2D.h>

#include <Entity.h>


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

        void on_collision(Entity* _with) override;

        void apply_input() override { }

    public:
        void set_lifetime(float _seconds);

    public:
        bool should_be_destroyed() const override;

    };

    class Projectile_Stub : public LEti::Rigid_Body_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LV::Variable_Base* M_construct_product() const override { return new Projectile; }

    };

}


#endif // PROJECTILE_H
