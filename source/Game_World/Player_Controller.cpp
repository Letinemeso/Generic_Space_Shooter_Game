#include <Game_World/Player_Controller.h>

using namespace GSSG;


void Player_Controller::M_create_player()
{
    m_player = (Player*)m_player_stub->construct();
    m_player->set_projectile_stub(m_player_projectile_stub);
    m_player->inject_camera(m_camera);
    m_player->inject_entity_manager(m_entity_manager);
    m_player->inject_player_controller(this);

    m_player->inject_eliminations_amount_caption(m_player_eliminations_amount_tf);

    m_entity_manager->add_entity(m_player);

//    m_player_respawn_tf->draw_module()->set_visible(false);
}



void Player_Controller::forcefuly_kill_player()
{
    if(!m_player)
        return;

    m_player->kill_force();
    m_entity_manager->remove_dead_entities();
}

void Player_Controller::notify_about_player_death()
{
    m_player = nullptr;

    unsigned int respawn_time = 1.0f;

    m_respawn_timer.start((float)respawn_time);
}

void Player_Controller::update(float _dt)
{
    if(m_player)
        return;

    m_respawn_timer.update(_dt);

    m_player_respawn_tf->text = "Respawning in " + std::to_string((unsigned int)(m_respawn_timer.time_left()) + 1) + "...";

    if(!m_respawn_timer.is_active())
    {
        M_create_player();
        m_player_respawn_tf->text = "";
    }
}
