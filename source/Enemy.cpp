#include <Enemy.h>

using namespace GSSG;

INIT_FIELDS(GSSG::Enemy, GSSG::Space_Ship);
FIELDS_END;


Enemy::Enemy()
    : Space_Ship()
{
//    Selector* root = new Selector;
    Sequence* root = new Sequence;
    root->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_find_closest_enemy)));
    root->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_look_away_from_closest_enemy)));
    m_behavior = root;
}

Enemy::~Enemy()
{
    delete m_behavior;
}



//  Enemy Behavior Logic

BT_Execution_Result Enemy::M_find_closest_enemy()
{
    if(m_attacked_entity)
    {
        float dist = LEti::Math::vector_length(get_pos() - m_attacked_entity->get_pos());
        if(dist < 300.0f)
            return BT_Execution_Result::Success;
    }

    const LDS::List<Entity*>& entities = m_entity_manager->registred_entities();

    m_attacked_entity = nullptr;
    float min_dist = -1.0f;
    Space_Ship* closest = nullptr;

    for(auto it = entities.begin(); !it.end_reached() && it.is_ok(); ++it)
    {
        Space_Ship* maybe_spaceship = LV::cast_variable<Space_Ship>(*it);

        if(!maybe_spaceship)
            continue;

        if(maybe_spaceship == this)
            continue;

        float dist = LEti::Math::vector_length(get_pos() - maybe_spaceship->get_pos());

        if(dist > 300.0f)
            continue;

        if(dist < min_dist || min_dist < 0.0f)
        {
            min_dist = dist;
            closest = maybe_spaceship;
        }
    }

    if(min_dist < 0.0f)
        return BT_Execution_Result::Fail;

    m_attacked_entity = closest;
    return BT_Execution_Result::Success;
}

//  Enemy Flee Behavior

BT_Execution_Result Enemy::M_is_low_hp()
{
    if(health() <= 2)
        return BT_Execution_Result::Success;
    return BT_Execution_Result::Fail;
}

BT_Execution_Result Enemy::M_look_away_from_closest_enemy()
{
    if(m_attacked_entity == nullptr)
        return BT_Execution_Result::Fail;

    glm::vec3 this_to_other_vec = get_pos() - m_attacked_entity->get_pos();

    if(LEti::Math::vector_length(this_to_other_vec) > 300.0f)
        return BT_Execution_Result::Fail;

    glm::vec3 look_direction = LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    float cos_between_vecs = LEti::Math::angle_cos_between_vectors(this_to_other_vec, look_direction);

    if(cos_between_vecs >= 0.0f)
        return BT_Execution_Result::Success;

    float angle = acos(cos_between_vecs);
    if(angle > LEti::Math::HALF_PI)
        angle -= LEti::Math::HALF_PI;

//    std::cout << angle << "\n";

    set_rotation_angle(LEti::Math::PI - angle);

    if(LEti::Math::dot_product(this_to_other_vec, look_direction) < 0.0f)
        set_rotation_angle(get_rotation_angle() + LEti::Math::PI);

//    if(cos_between_vecs)
//        rotate(acos(cos_between_vecs));


//    if(LEti::Math::dot_product(this_to_other_vec, look_direction) < 0.0f)
//    {
//        set_rotation_angle(acos(LEti::Math::angle_cos_between_vectors(this_to_other_vec, look_direction)));

//        look_direction = LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

//        if(LEti::Math::dot_product(this_to_other_vec, look_direction) < 0.0f)
//            set_rotation_angle(get_rotation_angle() + LEti::Math::PI);
//    }

    return BT_Execution_Result::Success;
}



BT_Execution_Result Enemy::M_process_idle_behavior()
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

    return BT_Execution_Result::Success;
}



void Enemy::apply_input()
{
//    switch(m_mode)
//    {
//    case(Mode::attack):
//    {
//        m_shoot_timer.update();
//        if(!m_shoot_timer.is_active())
//        {
//            m_shoot_timer.start(1.0f);
//            M_shoot();
//        }
//        break;
//    }
//    case(Mode::idle):
//    {
//        M_process_idle_behavior();
//        break;
//    }
//    case(Mode::flee):
//    {
//        break;
//    }
//    }

    m_behavior->process();

    m_shoot_timer.update();
    if(!m_shoot_timer.is_active())
    {
        m_shoot_timer.start(1.0f);
        M_shoot();
    }
}


void Enemy::update(float _ratio)
{
    Space_Ship::update(_ratio);
}
