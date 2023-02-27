#ifndef PLAYER_H
#define PLAYER_H


#include <Data_Structures/List.h>

#include <Event_Controller.h>

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

        LEti::Timer m_shoot_timer;

    public:
        Player();
        ~Player();

    public:
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_player_controller(Player_Controller* _player_controller) { m_player_controller = _player_controller; }

    public:
        void apply_input() override;
        void update(float _ratio = 1.0f) override;
        void on_delete_other_entity(const Entity* _entity_to_delete) override;

    };

}


#endif // PLAYER_H
