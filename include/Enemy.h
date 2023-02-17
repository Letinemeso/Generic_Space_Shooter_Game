#ifndef ENEMY_H
#define ENEMY_H

#include <Space_Ship.h>


namespace GSSG
{

    class Enemy : public Space_Ship
    {
    private:
        enum class Mode
        {
            idle,
            attack,
            flee
        };

        Mode m_mode = Mode::idle;

    private:
        float m_idle_rotation = 0.0f;
        float m_idle_acceleration = 0.0f;

    private:
        LEti::Timer m_shoot_timer;
        LEti::Timer m_idle_timer;

    private:
        void M_process_idle_behavior();

    public:
        void apply_input() override;

        void update(float _ratio = 1.0f) override;

    };

}


#endif // ENEMY_H
