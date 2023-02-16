#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Timer.h>
#include <Physics/Collision_Detector_2D.h>

#include <Entity.h>


namespace GSSG
{

    class Projectile : public Entity
    {
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

    public:
        void set_lifetime(float _seconds);

    public:
        bool should_be_destroyed() const override;

    };

}


#endif // PROJECTILE_H
