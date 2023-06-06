#include <Game_World/Game_World.h>

using namespace GSSG;


void Game_World::update()
{
    m_entity_manager->update_entities_prev_state();
    m_entity_manager->apply_entities_input();
    m_entity_manager->update_entities();

    m_gui->update_prev_state();

    m_collision_detector->update();
    m_collision_resolver->resolve_all(m_collision_detector->get_collisions__models());

    m_entity_manager->remove_dead_entities();
    m_enemy_generator->update();
    m_player_controller->update();
    m_effects_controller->update();

    m_gui->update();

    m_background->update();

    m_renderer->draw(*m_background->draw_module());

    m_entity_manager->draw_entities();
    m_effects_controller->draw();

    m_gui->draw();

}
