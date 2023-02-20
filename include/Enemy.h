#ifndef ENEMY_H
#define ENEMY_H

#include <Behavior_Tree/Selector.h>
#include <Behavior_Tree/Sequence.h>
#include <Behavior_Tree/Action.h>
#include <Space_Ship.h>


namespace GSSG
{

    class Enemy : public Space_Ship
    {
    public:
        DECLARE_VARIABLE;

    private:
        enum class Mode
        {
            idle,
            attack,
            flee
        };

        Mode m_mode = Mode::idle;

        Node_Base* m_behavior = nullptr;

        Space_Ship* m_attacked_entity = nullptr;

    public:
        Enemy();
        ~Enemy();

    private:
        float m_idle_rotation = 0.0f;
        float m_idle_acceleration = 0.0f;

    private:
        LEti::Timer m_shoot_timer;
        LEti::Timer m_idle_timer;

    private:    //  behavior logic
        BT_Execution_Result M_find_closest_enemy();

        //  flee
        BT_Execution_Result M_is_low_hp();
        BT_Execution_Result M_look_away_from_closest_enemy();
        BT_Execution_Result M_flee_from_enemy();

        //  attack
        BT_Execution_Result M_rotate_towards_enemy();
        BT_Execution_Result M_shoot_at_enemy();

        //  idle
        BT_Execution_Result M_process_idle_behavior();

    public:
        void apply_input() override;

        void update(float _ratio = 1.0f) override;

    };

}


#endif // ENEMY_H
