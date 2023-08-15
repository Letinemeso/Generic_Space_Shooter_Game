#include <Game_World/Space_Ship.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later

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
    LPhys::Rigid_Body_2D* pm = (LPhys::Rigid_Body_2D*)projectile->physics_module();

    glm::vec3 projectile_impulse = LEti::Math::rotate_vector({500.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, current_state().rotation().z);

    projectile->current_state().set_position(current_state().position());
    projectile->current_state().set_rotation(current_state().rotation());
    projectile->set_parent(this);
    projectile->set_lifetime(1.5f);
    pm->set_velocity(projectile_impulse);

    projectile->update_previous_state();
    projectile->update();

    m_entity_manager->add_entity(projectile);
return;
    physics_module()->apply_linear_impulse(-projectile_impulse * (pm->mass() / physics_module()->mass()));
    if(LEti::Math::vector_length(physics_module()->velocity()) > max_speed())
        physics_module()->set_velocity(LEti::Math::extend_vector_to_length(physics_module()->velocity(), max_speed()));
}



//bool Space_Ship::should_be_destroyed() const
//{

//}



void Space_Ship::update()
{
    Entity::update();
}
