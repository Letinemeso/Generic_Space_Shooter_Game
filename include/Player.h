#ifndef PLAYER_H
#define PLAYER_H


#include <Data_Structures/List.h>

#include <Event_Controller.h>
#include <Object_System/Text_Field.h>

#include <Enemy_Generator.h>
#include <Entity_Manager.h>
#include <Entity.h>
#include <Projectile.h>
#include <Space_Ship.h>


namespace GSSG
{

    class Player_Controller;

    class Player : public GSSG::Space_Ship
    {
    public:
        DECLARE_VARIABLE;

    private:
        LEti::Camera_2D* m_camera = nullptr;
        Player_Controller* m_player_controller = nullptr;

        LST::Timer m_shoot_timer;

        LEti::Text_Field* m_player_hp_tf = nullptr;

    private:
        LEti::Text_Field* m_eliminations_amount_tf = nullptr;
        unsigned int m_eliminations_amount = 0;

    public:
        Player();
        ~Player();

    public:
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_player_controller(Player_Controller* _player_controller) { m_player_controller = _player_controller; }
        void inject_player_hp_caption(LEti::Text_Field* _player_hp_tf);
        void inject_eliminations_amount_caption(LEti::Text_Field* _eliminations_amount_tf);

    public:
        void temp_apply_simple_input();
        void apply_input() override;
        void update(float _ratio = 1.0f) override;
        void on_other_entity_death(const Entity* _entity_to_delete) override;
        void on_collision(Entity* _with) override;
        void on_death() override;

    public:
        void set_eliminations_amount(unsigned int _amount);

    public:
        inline unsigned int eliminations_amount() const { return m_eliminations_amount; }

    };

    class Player_Stub : public Entity_Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LV::Variable_Base* M_construct_product() const override { return new Player; }

    };

}


#endif // PLAYER_H
