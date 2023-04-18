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
#include <GUI.h>
#include <Player.h>
#include <Enemy.h>
#include <Projectile.h>


namespace GSSG
{

    class Game_World : public Game_Logic
    {
    private:
        GSSG::Entity_Manager m_entity_manager;
        GSSG::GUI m_gui;

    public:
        void init();

    public:
        void update_previous_state() override;
        void update() override;
        void apply_input() override;
        void draw() override;

    };

}


#endif // GAME_WORLD_H
