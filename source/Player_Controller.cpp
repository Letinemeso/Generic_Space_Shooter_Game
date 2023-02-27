#include <Player_Controller.h>

using namespace GSSG;


void Player_Controller::M_create_player()
{
    m_player = new Player;
    m_player->init(*m_player_stub);
    m_player->set_projectile_stub(m_player_projectile_stub);
    m_player->inject_camera(m_camera);
    m_player->inject_entity_manager(m_entity_manager);
    m_player->inject_player_controller(this);
    m_player->set_health(5);
    m_player->set_mass(10.0f);
    m_player->inject_player_hp_caption(m_player_hp_tf);

    m_entity_manager->add_entity(m_player);

    m_player_respawn_tf->draw_module()->set_visible(false);
}



void Player_Controller::notify_about_player_death()
{
    m_player = nullptr;
    m_respawn_timer.start(5.0f);
    m_player_respawn_tf->set_text("Respawning in 5...");
    m_player_respawn_tf->draw_module()->set_visible(true);
}

void Player_Controller::update()
{
    if(m_player)
        return;

    m_respawn_timer.update();

    m_player_respawn_tf->set_text("Respawning in " + std::to_string((unsigned int)(m_respawn_timer.time_left()) + 1) + "...");

    if(!m_respawn_timer.is_active())
        M_create_player();
}
