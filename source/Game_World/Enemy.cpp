#include <Game_World/Enemy.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later

using namespace GSSG;


INIT_FIELDS(GSSG::Enemy, GSSG::Space_Ship)
FIELDS_END

INIT_FIELDS(GSSG::Enemy_Stub, GSSG::Entity_Stub)
FIELDS_END


Enemy::Enemy()
    : Space_Ship()
{
//    LGL::Selector* root = new LGL::Selector;
    LGL::Selector* root = new LGL::Selector;
    m_behavior = root;

//    Sequence* flee_subroot = new Sequence;
//    flee_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_find_closest_enemy();})) );
//    flee_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_is_low_hp();} )) );
//    flee_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_rotate_away_from_enemy();} )) );
//    flee_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_accelerate();} )) );
//    root->add_child(flee_subroot);

//    Sequence* attack_subroot = new Sequence;
//    attack_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_find_closest_enemy();} )) );
//    attack_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_rotate_towards_enemy();} )) );
//    attack_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_get_close_to_enemy();} )) );
//    attack_subroot->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_shoot_at_enemy();} )));
//    root->add_child(attack_subroot);

//    root->add_child( new Action( LST::Function<LGL::BT_Execution_Result()>([this](){return M_process_idle_behavior();} )) );


    root->add_child( new LGL::Action( LST::Function<LGL::BT_Execution_Result()>([this](){ return /*M_shoot_at_enemy();*/ LGL::BT_Execution_Result::Success; } )));
}

Enemy::~Enemy()
{
    delete m_behavior;
}



//  Enemy Behavior Logic

LGL::BT_Execution_Result Enemy::M_find_closest_enemy()
{
    if(m_attacked_entity)
    {
        float dist = LEti::Math::vector_length(current_state().position() - m_attacked_entity->current_state().position());
        if(dist < m_max_dist)
            return LGL::BT_Execution_Result::Success;
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

        float dist = LEti::Math::vector_length(current_state().position() - maybe_spaceship->current_state().position());

        if(dist > m_max_dist)
            continue;

        if(dist < min_dist || min_dist < 0.0f)
        {
            min_dist = dist;
            closest = maybe_spaceship;
        }
    }

    if(min_dist < 0.0f)
        return LGL::BT_Execution_Result::Fail;

    m_attacked_entity = closest;
    return LGL::BT_Execution_Result::Success;
}

LGL::BT_Execution_Result Enemy::M_accelerate()
{
    glm::vec3 look_direction = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z);
    physics_module()->apply_linear_impulse(look_direction * m_dt_for_behavior);

    float speed = LEti::Math::vector_length(physics_module()->velocity());
    if(speed > max_speed())
        physics_module()->set_velocity(LEti::Math::extend_vector_to_length(physics_module()->velocity(), max_speed()));

    return LGL::BT_Execution_Result::Success;
}


//  Enemy Flee Behavior

LGL::BT_Execution_Result Enemy::M_is_low_hp()
{
    if(m_health <= 2)
        return LGL::BT_Execution_Result::Success;
    return LGL::BT_Execution_Result::Fail;
}

LGL::BT_Execution_Result Enemy::M_rotate_away_from_enemy()
{
    if(m_attacked_entity == nullptr)
        return LGL::BT_Execution_Result::Fail;

    glm::vec3 this_to_other_vec = current_state().position() - m_attacked_entity->current_state().position();

    if(LEti::Math::vector_length(this_to_other_vec) > m_max_dist)
        return LGL::BT_Execution_Result::Fail;

    glm::vec3 look_direction = LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z);

    float cos_between_vecs = LEti::Math::angle_cos_between_vectors(this_to_other_vec, look_direction);

    if(cos_between_vecs >= 0.0f)
    {
        if(!LEti::Math::floats_are_equal(cos_between_vecs, 1.0f, 0.2f))
            return LGL::BT_Execution_Result::In_Progress;

        physics_module()->set_angular_velocity(0.0f);
        return LGL::BT_Execution_Result::Success;
    }

    physics_module()->set_angular_velocity( LEti::Math::PI - acos(cos_between_vecs) );

    if(LEti::Geometry_2D::vec_points_left(look_direction, this_to_other_vec))
        physics_module()->set_angular_velocity(-physics_module()->angular_velocity());

    return LGL::BT_Execution_Result::In_Progress;
}


// Enemy Attack Behavior

LGL::BT_Execution_Result Enemy::M_rotate_towards_enemy()
{
    if(m_attacked_entity == nullptr)
        return LGL::BT_Execution_Result::Fail;

    glm::vec3 this_to_other_vec = current_state().position() - m_attacked_entity->current_state().position();

    if(LEti::Math::vector_length(this_to_other_vec) > m_max_dist)
        return LGL::BT_Execution_Result::Fail;

    glm::vec3 inverse_look_direction = -LEti::Math::rotate_vector({1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z);

    float cos_between_vecs = LEti::Math::angle_cos_between_vectors(this_to_other_vec, inverse_look_direction);

    if(LEti::Math::floats_are_equal(cos_between_vecs, 1.0f, 0.001f))
    {
        physics_module()->set_angular_velocity(0.0f);
        return LGL::BT_Execution_Result::Success;
    }

    physics_module()->set_angular_velocity( LEti::Math::PI - acos(cos_between_vecs) );

    if(LEti::Geometry_2D::vec_points_left(inverse_look_direction, this_to_other_vec))
        physics_module()->set_angular_velocity(-physics_module()->angular_velocity());

    return LGL::BT_Execution_Result::In_Progress;
}

LGL::BT_Execution_Result Enemy::M_get_close_to_enemy()
{
    glm::vec3 impulse = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z);

    float dist = LEti::Math::get_distance(current_state().position(), m_attacked_entity->current_state().position());
    if(dist < 150.0f)
        impulse *= -1.0f;

    physics_module()->apply_linear_impulse(impulse * m_dt_for_behavior);

    float speed = LEti::Math::vector_length(physics_module()->velocity());
    if(speed > max_speed())
        physics_module()->set_velocity(LEti::Math::extend_vector_to_length(physics_module()->velocity(), max_speed()));

    if(dist < 150.0f || dist > 300.0f)
        return LGL::BT_Execution_Result::In_Progress;

    return LGL::BT_Execution_Result::Success;
}

LGL::BT_Execution_Result Enemy::M_shoot_at_enemy()
{
    m_shoot_timer.update(m_dt_for_behavior);
    if(!m_shoot_timer.is_active())
    {
        m_shoot_timer.start(1.0f);
        M_shoot();
        return LGL::BT_Execution_Result::Success;
    }
    return LGL::BT_Execution_Result::In_Progress;
}



LGL::BT_Execution_Result Enemy::M_process_idle_behavior()
{
    m_idle_timer.update(m_dt_for_behavior);
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
        physics_module()->apply_rotation(m_idle_rotation * m_dt_for_behavior);
        if(physics_module()->angular_velocity() > max_rotation_speed())
            physics_module()->set_angular_velocity(max_rotation_speed());
    }
    else if(!LEti::Math::floats_are_equal(physics_module()->angular_velocity(), 0.0f))
    {
        float multiplier = physics_module()->angular_velocity() < 0.0f ? 1.0f : -1.0f;
        physics_module()->set_angular_velocity(physics_module()->angular_velocity() + (rotation_acceleration() * multiplier * m_dt_for_behavior));
        if(fabs(physics_module()->angular_velocity()) < rotation_acceleration() * m_dt_for_behavior)
            physics_module()->set_angular_velocity(0.0f);
    }

    if(m_idle_acceleration > 0.0001f)
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({m_idle_acceleration, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z) * m_dt_for_behavior;
        physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(physics_module()->velocity());
        if(speed > max_speed())
            physics_module()->set_velocity(LEti::Math::extend_vector_to_length(physics_module()->velocity(), max_speed()));
    }
    else if(!LEti::Math::floats_are_equal(LEti::Math::vector_length(physics_module()->velocity()), 0.0f))
    {
        glm::vec3 impulse = -LEti::Math::extend_vector_to_length(physics_module()->velocity(), max_speed()) * m_dt_for_behavior;
        physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(physics_module()->velocity());
        if(speed < LEti::Math::vector_length(impulse))
            physics_module()->set_velocity({0.0f, 0.0f, 0.0f});
    }

    return LGL::BT_Execution_Result::Success;
}



void Enemy::on_collision(const LPhys::Intersection_Data &_id)
{
    Space_Ship::on_collision(_id);

    m_health -= 1;
}


void Enemy::apply_input(float _dt)
{
    m_dt_for_behavior = _dt;

    m_behavior->process();
}


void Enemy::update(float _dt)
{
    Space_Ship::update(_dt);
}


void Enemy::on_other_entity_death(const Entity *_entity_to_delete)
{
    Space_Ship::on_other_entity_death(_entity_to_delete);
    if(m_attacked_entity == _entity_to_delete)
        m_attacked_entity = nullptr;
}
