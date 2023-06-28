#include <Game_World/Space_Ship.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Space_Ship, GSSG::Entity);
FIELDS_END;


Space_Ship::Space_Ship()
{

}

Space_Ship::~Space_Ship()
{

}



void Space_Ship::M_shoot()
{
    L_ASSERT(m_projectile_stub);

    Projectile* projectile = (Projectile*)m_projectile_stub->construct();
    LEti::Physics_Module__Rigid_Body_2D* pm = (LEti::Physics_Module__Rigid_Body_2D*)projectile->physics_module();

    glm::vec3 projectile_impulse = LEti::Math::rotate_vector({500.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    projectile->set_pos(get_pos());
    projectile->set_rotation_angle(get_rotation_angle());
    projectile->set_parent(this);
    projectile->set_lifetime(1.5f);
    pm->set_velocity(projectile_impulse);

    projectile->update_previous_state();
    projectile->update(0.0f);

    m_entity_manager->add_entity(projectile);
return;
    M_get_physics_module()->apply_linear_impulse(-projectile_impulse * (pm->mass() / M_get_physics_module()->mass()));
    if(LEti::Math::vector_length(M_get_physics_module()->velocity()) > max_speed())
        M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));
}



//bool Space_Ship::should_be_destroyed() const
//{

//}



void Space_Ship::update(float _ratio)
{
    Entity::update(_ratio);
}
