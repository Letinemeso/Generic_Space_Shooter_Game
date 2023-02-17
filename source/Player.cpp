#include <Player.h>

using namespace GSSG;


Player::Player()
{

}

Player::~Player()
{

}



void Player::apply_input()
{
    L_ASSERT(m_projectile_stub);

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
    {
        apply_rotation(m_rotation_acceleration_speed * LEti::Event_Controller::get_dt());
        if(angular_velocity() > m_rotation_acceleration_speed * m_max_multiplier_limit)
            set_angular_velocity(m_rotation_acceleration_speed * m_max_multiplier_limit);
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
    {
        apply_rotation(-m_rotation_acceleration_speed * LEti::Event_Controller::get_dt());
        if(angular_velocity() < -m_rotation_acceleration_speed * m_max_multiplier_limit)
            set_angular_velocity(-m_rotation_acceleration_speed * m_max_multiplier_limit);
    }

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({m_acceleration_speed, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();

        if(LEti::Math::vector_length(velocity() + impulse) <= m_acceleration_speed * m_max_multiplier_limit)
            apply_linear_impulse(impulse);
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({-m_acceleration_speed, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();

        if(LEti::Math::vector_length(velocity() + impulse) <= m_acceleration_speed * m_max_multiplier_limit)
            apply_linear_impulse(impulse);
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_R))
    {
        set_velocity({0.0f, 0.0f, 0.0f});
        set_angular_velocity(0.0f);
    }

    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_SPACE))
    {
        Projectile* projectile = new Projectile;
        projectile->init(*m_projectile_stub);

        glm::vec3 projectile_impulse = LEti::Math::rotate_vector({500.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

        projectile->set_pos(get_pos()/* + projectile_impulse * 0.05f*/);
        projectile->set_rotation_angle(get_rotation_angle());
        projectile->set_velocity(projectile_impulse);
        projectile->set_parent(this);
        projectile->set_lifetime(1.0f);
        projectile->set_health(1);
        projectile->set_mass(1.0f);

        projectile->update_previous_state();
        projectile->update(0.0f);

        m_entity_manager->add_entity(projectile);

        apply_linear_impulse(-projectile_impulse * (projectile->mass() / mass()));
    }

}

void Player::update(float _ratio)
{
    Entity::update(_ratio);

    m_camera->set_position(get_pos());
}
