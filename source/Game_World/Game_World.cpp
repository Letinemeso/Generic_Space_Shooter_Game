#include <Game_World/Game_World.h>

using namespace GSSG;


void Game_World::update(float _dt)
{
    m_background->update(_dt);

    m_entity_manager->update_entities_prev_state();
    m_entity_manager->apply_entities_input(_dt);
    m_entity_manager->update_entities(_dt);

    m_gui->update_prev_state();

    m_collision_detector->update();
    m_collision_resolver->resolve_all(m_collision_detector->get_collisions__models());

    m_entity_manager->remove_dead_entities();
    m_enemy_generator->update(_dt);
    m_player_controller->update(_dt);
    m_effects_controller->update(_dt);

    m_gui->update(_dt);
}
