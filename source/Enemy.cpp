#include <Enemy.h>

using namespace GSSG;

INIT_FIELDS(GSSG::Enemy, GSSG::Space_Ship);
FIELDS_END;


Enemy::Enemy()
    : Space_Ship()
{
//    Selector* root = new Selector;
    Selector* root = new Selector;
    m_behavior = root;

    Sequence* flee_subroot = new Sequence;
    flee_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_find_closest_enemy)));
    flee_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_is_low_hp)));
    flee_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_rotate_away_from_enemy)));
    flee_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_accelerate)));
    root->add_child(flee_subroot);

    Sequence* attack_subroot = new Sequence;
    attack_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_find_closest_enemy)));
    attack_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_rotate_towards_enemy)));
    attack_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_get_close_to_enemy)));
    attack_subroot->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_shoot_at_enemy)));
    root->add_child(attack_subroot);

    root->add_child(new Action(LST::make_wrapper(*this, &Enemy::M_process_idle_behavior)));
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
        if(dist < m_max_dist)
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

        if(dist > m_max_dist)
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

BT_Execution_Result Enemy::M_accelerate()
{
    glm::vec3 look_direction = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());
    apply_linear_impulse(look_direction * LEti::Event_Controller::get_dt());

    float speed = LEti::Math::vector_length(velocity());
    if(speed > max_speed())
        set_velocity(LEti::Math::extend_vector_to_length(velocity(), max_speed()));

    return BT_Execution_Result::Success;
}


//  Enemy Flee Behavior

BT_Execution_Result Enemy::M_is_low_hp()
{
    if(health() <= 2)
        return BT_Execution_Result::Success;
    return BT_Execution_Result::Fail;
}

BT_Execution_Result Enemy::M_rotate_away_from_enemy()
{
    if(m_attacked_entity == nullptr)
        return BT_Execution_Result::Fail;

    glm::vec3 this_to_other_vec = get_pos() - m_attacked_entity->get_pos();

    if(LEti::Math::vector_length(this_to_other_vec) > m_max_dist)
        return BT_Execution_Result::Fail;

    glm::vec3 look_direction = LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    float cos_between_vecs = LEti::Math::angle_cos_between_vectors(this_to_other_vec, look_direction);

    if(cos_between_vecs >= 0.0f)
    {
        if(!LEti::Math::floats_are_equal(cos_between_vecs, 1.0f, 0.2f))
            return BT_Execution_Result::In_Progress;

        set_angular_velocity(0.0f);
        return BT_Execution_Result::Success;
    }

    set_angular_velocity( LEti::Math::PI - acos(cos_between_vecs) );

    if(LEti::Geometry_2D::vec_points_left(look_direction, this_to_other_vec))
        set_angular_velocity(-angular_velocity());

    return BT_Execution_Result::In_Progress;
}


// Enemy Attack Behavior

BT_Execution_Result Enemy::M_rotate_towards_enemy()
{
    if(m_attacked_entity == nullptr)
        return BT_Execution_Result::Fail;

    glm::vec3 this_to_other_vec = get_pos() - m_attacked_entity->get_pos();

    if(LEti::Math::vector_length(this_to_other_vec) > m_max_dist)
        return BT_Execution_Result::Fail;

    glm::vec3 inverse_look_direction = -LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    float cos_between_vecs = LEti::Math::angle_cos_between_vectors(this_to_other_vec, inverse_look_direction);

    if(LEti::Math::floats_are_equal(cos_between_vecs, 1.0f, 0.05f))
    {
        set_angular_velocity(0.0f);
        return BT_Execution_Result::Success;
    }

    set_angular_velocity( LEti::Math::PI - acos(cos_between_vecs) );

    if(LEti::Geometry_2D::vec_points_left(inverse_look_direction, this_to_other_vec))
        set_angular_velocity(-angular_velocity());

    return BT_Execution_Result::In_Progress;
}

BT_Execution_Result Enemy::M_get_close_to_enemy()
{
    glm::vec3 impulse = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    float dist = LEti::Math::get_distance(get_pos(), m_attacked_entity->get_pos());
    if(dist < 150.0f)
        impulse *= -1.0f;

    apply_linear_impulse(impulse * LEti::Event_Controller::get_dt());

    float speed = LEti::Math::vector_length(velocity());
    if(speed > max_speed())
        set_velocity(LEti::Math::extend_vector_to_length(velocity(), max_speed()));

    if(dist < 150.0f || dist > 300.0f)
        return BT_Execution_Result::In_Progress;

    return BT_Execution_Result::Success;
}

BT_Execution_Result Enemy::M_shoot_at_enemy()
{
    m_shoot_timer.update();
    if(!m_shoot_timer.is_active())
    {
        m_shoot_timer.start(1.0f);
        M_shoot();
        return BT_Execution_Result::Success;
    }
    return BT_Execution_Result::In_Progress;
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
}


void Enemy::update(float _ratio)
{
    Space_Ship::update(_ratio);
}
