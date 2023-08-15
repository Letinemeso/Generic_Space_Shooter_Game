#include <Game_World/Projectile.h>

#include <Game_World/Player.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Projectile, GSSG::Entity)
FIELDS_END

INIT_FIELDS(GSSG::Projectile_Stub, GSSG::Entity_Stub)
FIELDS_END



Projectile::Projectile()
{

}

Projectile::~Projectile()
{

}



void Projectile::update()
{
    Entity::update();

    m_time_until_death -= LR::Event_Controller::get_dt();
}



void Projectile::on_collision(const LPhys::Intersection_Data& _id)
{
    Entity::on_collision(_id);

    m_time_until_death = -1.0f;

//    Entity* with = (Entity*)(_id.first->associated_object() == this ? _id.second->associated_object() : _id.first->associated_object());
    Entity_Physics_Module* with = (Entity_Physics_Module*)(_id.first == physics_module() ? _id.second : _id.first );

    //  TODO: think of better way to inform player
    //  IDEA: make another projectile derived from this and override this method
    Player* maybe_player = LV::cast_variable<Player>((Entity*)parent());
    if(!maybe_player)
        return;

    if(!LV::cast_variable<Enemy>(with->owner()))
        return;

    maybe_player->set_eliminations_amount(maybe_player->eliminations_amount() + 1);
}



void Projectile::set_lifetime(float _seconds)
{
    m_time_until_death = _seconds;
    m_has_limited_lifetime = true;
}



bool Projectile::should_be_destroyed() const
{
    if(m_has_limited_lifetime)
        return m_time_until_death <= 0.0f;

    return false;
}
