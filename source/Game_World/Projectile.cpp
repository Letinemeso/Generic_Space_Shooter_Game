#include <Game_World/Projectile.h>

#include <Game_World/Player.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Projectile, GSSG::Entity)
FIELDS_END

INIT_FIELDS(GSSG::Projectile_Stub, GSSG::Entity_Stub)
FIELDS_END



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



void Projectile::on_collision(Entity* _with)
{
    Entity::on_collision(_with);

    if(_with->health() > 0)
        return;

    //  TODO: think of better way to inform player
    //  IDEA: make another projectile derived from this and override this method
    Player* maybe_player = LV::cast_variable<Player>((Entity*)parent());
    if(!maybe_player)
        return;

    if(!LV::cast_variable<Enemy>(_with))
        return;

    maybe_player->set_eliminations_amount(maybe_player->eliminations_amount() + 1);
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
