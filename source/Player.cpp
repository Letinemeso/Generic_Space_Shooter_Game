#include <Player.h>

using namespace GSSG;

INIT_FIELDS(GSSG::Player, GSSG::Space_Ship);
FIELDS_END;


Player::Player()
{

}

Player::~Player()
{

}



void Player::apply_input()
{
    bool has_rotational_input = false;

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
    {
        apply_rotation(rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(angular_velocity() > max_rotation_speed())
            set_angular_velocity(max_rotation_speed());
        has_rotational_input = true;
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
    {
        apply_rotation(-rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(angular_velocity() < -max_rotation_speed())
            set_angular_velocity(-max_rotation_speed());
        has_rotational_input = true;
    }

    if(!has_rotational_input && !LEti::Math::floats_are_equal(angular_velocity(), 0.0f))
    {
        float multiplier = angular_velocity() < 0.0f ? 1.0f : -1.0f;

        set_angular_velocity(angular_velocity() + (rotation_acceleration() * multiplier * LEti::Event_Controller::get_dt()));

        if(fabs(angular_velocity()) < rotation_acceleration() * LEti::Event_Controller::get_dt())
            set_angular_velocity(0.0f);
    }

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();

        move(impulse / 5.0f);

//        apply_linear_impulse(impulse);

//        float speed = LEti::Math::vector_length(velocity());
//        if(speed > max_speed())
//            set_velocity(LEti::Math::extend_vector_to_length(velocity(), max_speed()));
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({-acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();

        move(impulse / 5.0f);

//        apply_linear_impulse(impulse);

//        float speed = LEti::Math::vector_length(velocity());
//        if(speed > max_speed())
//            set_velocity(LEti::Math::extend_vector_to_length(velocity(), max_speed()));
    }

    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_SPACE))
    {
        M_shoot();
    }

}

void Player::update(float _ratio)
{
    Space_Ship::update(_ratio);

    m_camera->set_position(get_pos());
}
