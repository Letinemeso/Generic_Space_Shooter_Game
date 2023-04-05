#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <Object_System/Text_Field.h>

#include <Player.h>
#include <Entity_Manager.h>


namespace GSSG
{

    class Player_Controller
    {
    private:
        const LEti::Object_2D_Stub* m_player_projectile_stub = nullptr;
        const LEti::Object_2D_Stub* m_player_stub = nullptr;
        Player* m_player = nullptr;
        LST::Timer m_respawn_timer;

        LEti::Text_Field* m_player_hp_tf = nullptr;
        LEti::Text_Field* m_player_respawn_tf = nullptr;
        LEti::Text_Field* m_player_eliminations_amount_tf = nullptr;

    private:
        LEti::Camera_2D* m_camera = nullptr;
        Entity_Manager* m_entity_manager = nullptr;

    public:
        inline void set_projectile_stub(const LEti::Object_2D_Stub* _player_projectile_stub) { m_player_projectile_stub = _player_projectile_stub; }
        inline void set_player_stub(const LEti::Object_2D_Stub* _player_stub) { m_player_stub = _player_stub; }
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }
        inline void inject_player_hp_caption(LEti::Text_Field* _player_hp_tf) { m_player_hp_tf = _player_hp_tf; }
        inline void inject_player_respawn_caption(LEti::Text_Field* _player_respawn_tf) { m_player_respawn_tf = _player_respawn_tf; }
        inline void inject_player_eliminations_amount_caption(LEti::Text_Field* _player_eliminations_amount_tf) { m_player_eliminations_amount_tf = _player_eliminations_amount_tf; }

    private:
        void M_create_player();

    public:
        void notify_about_player_death();
        void update();

    };

}


#endif // PLAYER_CONTROLLER_H
