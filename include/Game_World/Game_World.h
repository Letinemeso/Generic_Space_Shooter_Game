#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <Collision_Detection/Collision_Detector_2D.h>
#include <Collision_Detection/Space_Hasher_2D.h>
#include <Collision_Detection/Dynamic_Narrow_CD.h>
#include <Collision_Detection/SAT_Narrowest_CD.h>
#include <Collision_Resolution//Collision_Resolver.h>

#include <Game_World/Collision_Resolution__Entity.h>
#include <Game_Logic.h>
#include <Game_World/Entity_Manager.h>
#include <Effects_Controller.h>
#include <Game_World/Player_Controller.h>
#include <GUI.h>
#include <Game_World/Background.h>
#include <Game_World/Player.h>
#include <Game_World/Enemy.h>
#include <Game_World/Projectile.h>


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
        LPhys::Collision_Detector_2D* m_collision_detector = nullptr;
        LPhys::Collision_Resolver* m_collision_resolver = nullptr;

    private:
        const LR::Renderer* m_renderer = nullptr;

    public:
        inline void set_entity_manager(GSSG::Entity_Manager* _ptr) { m_entity_manager = _ptr; }
        inline void set_enemy_generator(GSSG::Enemy_Generator* _ptr) { m_enemy_generator = _ptr; }
        inline void set_player_controller(GSSG::Player_Controller* _ptr) { m_player_controller = _ptr; }
        inline void set_effects_controller(GSSG::Effects_Controller* _ptr) { m_effects_controller = _ptr; }
        inline void set_gui(GSSG::GUI* _ptr) { m_gui = _ptr; }
        inline void set_background(GSSG::Background* _ptr) { m_background = _ptr; }
        inline void set_collision_detector(LPhys::Collision_Detector_2D* _ptr) { m_collision_detector = _ptr; }
        inline void set_collision_resolver(LPhys::Collision_Resolver* _ptr) { m_collision_resolver = _ptr; }
        inline void set_renderer(const LR::Renderer* _ptr) { m_renderer = _ptr; }

    public:
        void update() override;

    };

}


#endif // GAME_WORLD_H
