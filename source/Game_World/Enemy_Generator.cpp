#include <Game_World/Enemy_Generator.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later

using namespace GSSG;


Enemy_Generator::Enemy_Generator()
{

}

Enemy_Generator::~Enemy_Generator()
{

}



void Enemy_Generator::update()
{
    L_ASSERT(m_enemy_stub);

    m_timer.update(LR::Event_Controller::get_dt());
    if(!m_timer.is_active())
    {
        spawn_enemy();
        m_timer.start(m_spawn_frequency);
    }
}



void Enemy_Generator::spawn_enemy() const
{
    L_ASSERT(m_enemy_stub);

    glm::vec3 position = m_camera->position();
    position.x += LEti::Math::random_number(100, 400) * (LEti::Math::random_number(0, 2) == 0 ? -1.0f : 1.0f);
    position.y += LEti::Math::random_number(100, 400) * (LEti::Math::random_number(0, 2) == 0 ? -1.0f : 1.0f);

    float random_scale = LEti::Math::random_number(1, 6);

    Enemy* enemy = (Enemy*)m_enemy_stub->construct();
    enemy->set_projectile_stub(m_enemy_projectile_stub);
    enemy->inject_entity_manager(m_entity_manager);
    enemy->current_state().set_position(position);
    enemy->current_state().set_scale({5.0f * (float)random_scale, 5.0f * (float)random_scale, 5.0f * (float)random_scale}); //  to hell with multiplying same shit several times. ill probably rewrite this stuff later anyways
    ((LPhys::Rigid_Body_2D*)enemy->physics_module())->set_mass_multiplier(3.0f * (float)random_scale);
    enemy->current_state().set_rotation({0.0f, 0.0f, LEti::Math::DOUBLE_PI / 360.0f * (float)LEti::Math::random_number(0, 360)});

    //  color randomization will have to wait

//    for(unsigned int i=0; i<3; ++i)
//    {
//        float random_color = (float)LEti::Math::random_number(1, 11) / 10.0f;
//        for(unsigned int c=i; c<enemy->draw_module()->colors().size(); c += 4)
//            enemy->draw_module()->colors()[c] = random_color;
//    }

    enemy->update_previous_state();
    enemy->update();

    m_entity_manager->add_entity(enemy);
}
