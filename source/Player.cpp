#include <Player.h>

#include <Player_Controller.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Player, GSSG::Space_Ship)
FIELDS_END

INIT_FIELDS(GSSG::Player_Stub, GSSG::Entity_Stub)
FIELDS_END



Player::Player()
{

}

Player::~Player()
{

}



void Player::inject_player_hp_caption(LEti::Text_Field *_player_hp_tf)
{
    m_player_hp_tf = _player_hp_tf;
    m_player_hp_tf->set_text("HP " + std::to_string(health()));
}

void Player::inject_eliminations_amount_caption(LEti::Text_Field *_eliminations_amount_tf)
{
    m_eliminations_amount_tf = _eliminations_amount_tf;
    m_eliminations_amount_tf->set_text("Eliminations " + std::to_string(m_eliminations_amount));
}



void Player::apply_input()
{
    bool has_rotational_input = false;

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
    {
        M_get_physics_module()->apply_rotation(rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(M_get_physics_module()->angular_velocity() > max_rotation_speed())
            M_get_physics_module()->set_angular_velocity(max_rotation_speed());
        has_rotational_input = true;

//        move(glm::vec3{-300.0f, 0.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
    {
        M_get_physics_module()->apply_rotation(-rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(M_get_physics_module()->angular_velocity() < -max_rotation_speed())
            M_get_physics_module()->set_angular_velocity(-max_rotation_speed());
        has_rotational_input = true;

//        move(glm::vec3{300.0f, 0.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }

    if(!has_rotational_input && !LEti::Math::floats_are_equal(M_get_physics_module()->angular_velocity(), 0.0f))
    {
        float multiplier = M_get_physics_module()->angular_velocity() < 0.0f ? 1.0f : -1.0f;

        M_get_physics_module()->set_angular_velocity(M_get_physics_module()->angular_velocity() + (rotation_acceleration() * multiplier * LEti::Event_Controller::get_dt()));

        if(fabs(M_get_physics_module()->angular_velocity()) < rotation_acceleration() * LEti::Event_Controller::get_dt())
            M_get_physics_module()->set_angular_velocity(0.0f);
    }

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();
        M_get_physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(M_get_physics_module()->velocity());
        if(speed > max_speed())
            M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));

//        move(glm::vec3{0.0f, 300.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({-acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();
        M_get_physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(M_get_physics_module()->velocity());
        if(speed > max_speed())
            M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));

//        move(glm::vec3{0.0f, -300.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }

    m_shoot_timer.update(LEti::Event_Controller::get_dt());

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_SPACE) && !m_shoot_timer.is_active())
    {
        M_shoot();
        m_shoot_timer.start(0.6f);
    }

}

void Player::update(float _ratio)
{
    Space_Ship::update(_ratio);

    m_camera->set_position(get_pos());
}

void Player::on_other_entity_death(const Entity *_entity_to_delete)
{
    Space_Ship::on_other_entity_death(_entity_to_delete);
}

void Player::on_collision(Entity *_with)
{
    Space_Ship::on_collision(_with);

    if(health() > 0)
        m_player_hp_tf->set_text("HP " + std::to_string(health()));
    else
        m_player_hp_tf->set_text(" ");
}

void Player::on_death()
{
    Space_Ship::on_death();

    m_player_controller->notify_about_player_death();
}



void Player::set_eliminations_amount(unsigned int _amount)
{
    m_eliminations_amount = _amount;
    m_eliminations_amount_tf->set_text("Eliminations " + std::to_string(m_eliminations_amount));
}
