#include <Game_World/Entity.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later
#include <Collision_Detection/Intersection_Data.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Entity_Stub, LEti::Object_2D_Stub)

ADD_CHILD("draw_module", *draw_module)
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

    LR::Default_Draw_Module_2D* dm = (LR::Default_Draw_Module_2D*)draw_module->construct();

    Entity_Physics_Module* pm = (Entity_Physics_Module*)physics_module->construct();
    pm->set_on_alignment_func([pm, dm]()
    {
        dm->move_raw(-pm->calculate_raw_center_of_mass());
    });
    pm->align_to_center_of_mass();

    result->set_physics_module(pm);
    pm->set_owner(result);

    result->inject_effects_controller(effects_controller);
    result->set_on_death_effect(on_death_effect);

    result->add_module(pm);
    result->add_module(dm);
}



Entity_Stub::~Entity_Stub()
{
    delete physics_module;
    delete draw_module;
}



INIT_FIELDS(GSSG::Entity, LEti::Object_2D)
FIELDS_END


Entity::Entity()
{

}

Entity::~Entity()
{

}



void Entity::update(float _dt)
{
    LEti::Object_2D::update(_dt);
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

    Visual_Effect* effect = (Visual_Effect*)m_on_death_effect->construct();
    effect->current_state().set_scale(current_state().scale());
    effect->current_state().set_position(current_state().position());
    effect->current_state().set_rotation(current_state().rotation());
    m_effects_controller->add_object(effect);
}
