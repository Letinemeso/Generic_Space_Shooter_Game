#include <Game_World/Entity.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later
#include <Collision_Detection/Intersection_Data.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Entity_Stub, LEti::Object_2D_Stub)

ADD_CHILD("physics_module", *physics_module)

FIELDS_END


LV::Variable_Base* Entity_Stub::M_construct_product() const
{
    return new Entity;
}

void Entity_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    LEti::Object_2D_Stub::M_init_constructed_product(_product);

    Entity* result = (Entity*)_product;

    Entity_Physics_Module* pm = (Entity_Physics_Module*)physics_module->construct();
    pm->set_on_alignment_func([result, pm]()
    {
        result->draw_module()->move_raw(-pm->calculate_raw_center_of_mass());
    });
    pm->align_to_center_of_mass();

    result->add_module(pm);
    result->set_physics_module(pm);
    pm->set_owner(result);

    result->inject_effects_controller(effects_controller);
    result->set_on_death_effect(on_death_effect);
}



Entity_Stub::~Entity_Stub()
{
    delete physics_module;
}



INIT_FIELDS(GSSG::Entity, LEti::Object_2D)
FIELDS_END


Entity::Entity()
{

}

Entity::~Entity()
{

}



void Entity::update()
{
    LEti::Object_2D::update();
//    m_physics_module->update(LR::Event_Controller::get_dt());
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
    effect->set_scale(get_scale()/* * 2.0f*/);
    effect->set_pos(get_pos());
    effect->set_rotation_angle(get_rotation_angle());
    m_effects_controller->add_object(effect);
}
