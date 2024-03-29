#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <Game_World/Player.h>
#include <Game_World/Entity_Manager.h>


namespace GSSG
{

    class Player_Controller
    {
    private:
        const LEti::Object_2D_Stub* m_player_projectile_stub = nullptr;
        const Player_Stub* m_player_stub = nullptr;
        Player* m_player = nullptr;
        LST::Timer m_respawn_timer;

        LR::Text_Field_Settings* m_player_respawn_tf = nullptr;
        LR::Text_Field_Settings* m_player_eliminations_amount_tf = nullptr;

    private:
        LR::Camera_2D* m_camera = nullptr;
        Entity_Manager* m_entity_manager = nullptr;

    public:
        inline void set_projectile_stub(const LEti::Object_2D_Stub* _player_projectile_stub) { m_player_projectile_stub = _player_projectile_stub; }
        inline void set_player_stub(const Player_Stub* _player_stub) { m_player_stub = _player_stub; }
        inline void inject_camera(LR::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }
        inline void inject_player_respawn_caption(LR::Text_Field_Settings* _player_respawn_tf) { m_player_respawn_tf = _player_respawn_tf; }
        inline void inject_player_eliminations_amount_caption(LR::Text_Field_Settings* _player_eliminations_amount_tf) { m_player_eliminations_amount_tf = _player_eliminations_amount_tf; }

    private:
        void M_create_player();

    public:
        void forcefuly_kill_player();
        void notify_about_player_death();
        void update(float _dt);

    };

}


#endif // PLAYER_CONTROLLER_H
