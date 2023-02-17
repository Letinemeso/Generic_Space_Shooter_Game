#include <Enemy.h>

using namespace GSSG;


void Enemy::M_process_idle_behavior()
{
    m_idle_timer.update();
    if(!m_idle_timer.is_active())
    {
        if(m_idle_acceleration > 0.001f)
        {
            m_idle_acceleration = 0.0f;
            m_idle_rotation = ((float)LEti::Math::random_number(1, 10) / 10.0f) * (max_rotation_speed() * 0.5f);
            if(LEti::Math::random_number(0, 2) == 0)
                m_idle_rotation *= -1.0f;
        }
        else
        {
            m_idle_rotation = 0.0f;
            m_idle_acceleration = ((float)LEti::Math::random_number(1, 10) / 10.0f) * (max_speed() * 0.5f);
        }

        m_idle_timer.start(2.0f);
    }

    if(fabs(m_idle_rotation) > 0.0001f)
    {
        apply_rotation(m_idle_rotation * LEti::Event_Controller::get_dt());
        if(angular_velocity() > max_rotation_speed())
            set_angular_velocity(max_rotation_speed());
    }
    else if(!LEti::Math::floats_are_equal(angular_velocity(), 0.0f))
    {
        float multiplier = angular_velocity() < 0.0f ? 1.0f : -1.0f;
        set_angular_velocity(angular_velocity() + (rotation_acceleration() * multiplier * LEti::Event_Controller::get_dt()));
        if(fabs(angular_velocity()) < rotation_acceleration() * LEti::Event_Controller::get_dt())
            set_angular_velocity(0.0f);
    }

    if(m_idle_acceleration > 0.0001f)
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({m_idle_acceleration, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();
        apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(velocity());
        if(speed > max_speed())
            set_velocity(LEti::Math::extend_vector_to_length(velocity(), max_speed()));
    }
    else if(!LEti::Math::floats_are_equal(LEti::Math::vector_length(velocity()), 0.0f))
    {
        glm::vec3 impulse = -LEti::Math::extend_vector_to_length(velocity(), max_speed()) * LEti::Event_Controller::get_dt();
        apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(velocity());
        if(speed < LEti::Math::vector_length(impulse))
            set_velocity({0.0f, 0.0f, 0.0f});
    }


}



void Enemy::apply_input()
{
    switch(m_mode)
    {
    case(Mode::attack):
    {
        m_shoot_timer.update();
        if(!m_shoot_timer.is_active())
        {
            m_shoot_timer.start(1.0f);
            M_shoot();
        }
        break;
    }
    case(Mode::idle):
    {
        M_process_idle_behavior();
        break;
    }
    case(Mode::flee):
    {
        break;
    }
    }


}


void Enemy::update(float _ratio)
{
    Space_Ship::update(_ratio);
}
