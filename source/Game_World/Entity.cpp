#include <Game_World/Entity.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Entity_Stub, LEti::Rigid_Body_2D_Stub)
FIELDS_END


LV::Variable_Base* Entity_Stub::M_construct_product() const
{
    return new Entity;
}

void Entity_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    LEti::Rigid_Body_2D_Stub::M_init_constructed_product(_product);

    Entity* result = (Entity*)_product;

    result->inject_effects_controller(effects_controller);
    result->set_on_death_effect(on_death_effect);
    result->set_health(health);
}



INIT_FIELDS(GSSG::Entity, LEti::Object_2D)
FIELDS_END


Entity::Entity()
{

}

Entity::~Entity()
{

}


void Entity::on_other_entity_death(const Entity *_entity_to_delete)
{
    if(m_parent == _entity_to_delete)
        m_parent = nullptr;
}

void Entity::on_death()
{
    if(!m_effects_controller || !m_on_death_effect)
        return;

    LEti::Object_2D* effect = (LEti::Object_2D*)m_on_death_effect->construct();
    effect->set_scale(get_scale() * 2.0f);
    effect->set_pos(get_pos());
    effect->set_rotation_angle(get_rotation_angle());
    m_effects_controller->add_object(effect);
}
