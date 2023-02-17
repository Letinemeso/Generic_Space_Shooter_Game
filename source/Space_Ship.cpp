#include <Space_Ship.h>

using namespace GSSG;


Space_Ship::Space_Ship()
{

}

Space_Ship::~Space_Ship()
{

}



void Space_Ship::M_shoot()
{
    L_ASSERT(m_projectile_stub);

    Projectile* projectile = new Projectile;
    projectile->init(*m_projectile_stub);

    glm::vec3 projectile_impulse = LEti::Math::rotate_vector({500.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    projectile->set_pos(get_pos());
    projectile->set_rotation_angle(get_rotation_angle());
    projectile->set_velocity(projectile_impulse);
    projectile->set_parent(this);
    projectile->set_lifetime(1.5f);
    projectile->set_health(1);
    projectile->set_mass(1.0f);

    projectile->update_previous_state();
    projectile->update(0.0f);

    m_entity_manager->add_entity(projectile);

    apply_linear_impulse(-projectile_impulse * (projectile->mass() / mass()));
}



void Space_Ship::update(float _ratio)
{
    Entity::update(_ratio);
}
