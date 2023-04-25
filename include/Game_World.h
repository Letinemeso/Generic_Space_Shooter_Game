#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <Physics/Collision_Detector_2D.h>
#include <Physics/Space_Hasher_2D.h>
#include <Physics/Dynamic_Narrow_CD.h>
#include <Physics/SAT_Narrowest_CD.h>
#include <Physics/Collision_Resolver.h>

#include <Collision_Resolution__Entity.h>
#include <Game_Logic.h>
#include <Entity_Manager.h>
#include <Effects_Controller.h>
#include <Player_Controller.h>
#include <GUI.h>
#include <Background.h>
#include <Player.h>
#include <Enemy.h>
#include <Projectile.h>


namespace GSSG
{

    class Game_World : public Game_Logic
    {
    private:
        GSSG::Entity_Manager* m_entity_manager = nullptr;
        GSSG::Enemy_Generator* m_enemy_generator = nullptr;
        GSSG::Player_Controller* m_player_controller = nullptr;
        GSSG::Effects_Controller* m_effects_controller = nullptr;

        GSSG::GUI* m_gui = nullptr;

        GSSG::Background* m_background = nullptr;

    private:
        LEti::Collision_Detector_2D* m_collision_detector = nullptr;
        LEti::Collision_Resolver* m_collision_resolver = nullptr;

    private:
        const LEti::Renderer* m_renderer = nullptr;

    public:
        inline void set_entity_manager(GSSG::Entity_Manager* _ptr) { m_entity_manager = _ptr; }
        inline void set_enemy_generator(GSSG::Enemy_Generator* _ptr) { m_enemy_generator = _ptr; }
        inline void set_player_controller(GSSG::Player_Controller* _ptr) { m_player_controller = _ptr; }
        inline void set_effects_controller(GSSG::Effects_Controller* _ptr) { m_effects_controller = _ptr; }
        inline void set_gui(GSSG::GUI* _ptr) { m_gui = _ptr; }
        inline void set_background(GSSG::Background* _ptr) { m_background = _ptr; }
        inline void set_collision_detector(LEti::Collision_Detector_2D* _ptr) { m_collision_detector = _ptr; }
        inline void set_collision_resolver(LEti::Collision_Resolver* _ptr) { m_collision_resolver = _ptr; }
        inline void set_renderer(const LEti::Renderer* _ptr) { m_renderer = _ptr; }

    public:
        void update() override;

    };

}


#endif // GAME_WORLD_H
