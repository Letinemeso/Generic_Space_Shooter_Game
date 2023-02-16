#include <Projectile.h>

using namespace GSSG;


Projectile::Projectile()
{
    m_health = 1;
}

Projectile::~Projectile()
{

}



void Projectile::revert_to_previous_state()
{
    Entity::revert_to_previous_state();

    m_time_until_death = m_time_until_death_prev_state;
}

void Projectile::update_previous_state()
{
    Entity::update_previous_state();

    m_time_until_death_prev_state = m_time_until_death;
}

void Projectile::update(float _ratio)
{
    Entity::update(_ratio);

    m_time_until_death -= LEti::Event_Controller::get_dt() * _ratio;
}



void Projectile::set_lifetime(float _seconds)
{
    m_time_until_death_prev_state = _seconds;
    m_time_until_death = _seconds;
    m_has_limited_lifetime = true;
}



bool Projectile::should_be_destroyed() const
{
    if(Entity::should_be_destroyed())
        return true;

    if(m_has_limited_lifetime)
        return m_time_until_death <= 0.0f;

    return false;
}
