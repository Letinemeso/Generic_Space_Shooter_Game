#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

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
        LEti::Timer m_respawn_timer;

    private:
        LEti::Camera_2D* m_camera = nullptr;
        Entity_Manager* m_entity_manager = nullptr;

    public:
        inline void set_projectile_stub(const LEti::Object_2D_Stub* _player_projectile_stub) { m_player_projectile_stub = _player_projectile_stub; }
        inline void set_player_stub(const LEti::Object_2D_Stub* _player_stub) { m_player_stub = _player_stub; }
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }

    private:
        void M_create_player();

    public:
        void notify_about_player_death();
        void update();

    };

}


#endif // PLAYER_CONTROLLER_H
