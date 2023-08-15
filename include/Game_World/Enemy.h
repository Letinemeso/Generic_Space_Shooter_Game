#ifndef ENEMY_H
#define ENEMY_H

#include <Stuff/Timer.h>

#include <Behavior_Tree/Selector.h>
#include <Behavior_Tree/Sequence.h>
#include <Behavior_Tree/Action.h>
#include <Game_World/Space_Ship.h>


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

        LGL::Node_Base* m_behavior = nullptr;
        float m_dt_for_behavior = 0.0f; //  bruh

        Space_Ship* m_attacked_entity = nullptr;
        float m_max_dist = 400.0f;

        int m_health = 1;   //  temporary stuff

    public:
        Enemy();
        ~Enemy();

        inline void temp_set_health(int _health) { m_health = _health; }

    private:
        float m_idle_rotation = 0.0f;
        float m_idle_acceleration = 0.0f;

    private:
        LST::Timer m_shoot_timer;
        LST::Timer m_idle_timer;

    private:    //  behavior logic
        LGL::BT_Execution_Result M_find_closest_enemy();
        LGL::BT_Execution_Result M_accelerate();

        //  flee
        LGL::BT_Execution_Result M_is_low_hp();
        LGL::BT_Execution_Result M_rotate_away_from_enemy();

        //  attack
        LGL::BT_Execution_Result M_rotate_towards_enemy();
        LGL::BT_Execution_Result M_get_close_to_enemy();
        LGL::BT_Execution_Result M_shoot_at_enemy();

        //  idle
        LGL::BT_Execution_Result M_process_idle_behavior();

    public:
        inline bool should_be_destroyed() const override { return m_health <= 0; }

    public:
        void on_collision(const LPhys::Intersection_Data& _id) override;

        void apply_input(float _dt) override;

        void update(float _dt) override;

        void on_other_entity_death(const Entity* _entity_to_delete) override;

    };

    class Enemy_Stub : public Entity_Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LV::Variable_Base* M_construct_product() const override { return new Enemy; }

    };

}


#endif // ENEMY_H
