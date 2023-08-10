#include <Game_World/Collision_Resolution__Entity.h>

using namespace GSSG;


bool Collision_Resolution__Entity::resolve(const LPhys::Intersection_Data &_id)
{
    Entity_Physics_Module* maybe_entity_1_pm = LV::cast_variable<Entity_Physics_Module>((LPhys::Physics_Module_2D*)_id.first);
    Entity_Physics_Module* maybe_entity_2_pm = LV::cast_variable<Entity_Physics_Module>((LPhys::Physics_Module_2D*)_id.second);

    if(!maybe_entity_1_pm || !maybe_entity_2_pm)
        return false;

    Entity* entity_1 = maybe_entity_1_pm->owner();
    Entity* entity_2 = maybe_entity_2_pm->owner();

    if(!entity_1 || !entity_2)
        return false;

    if(entity_1->parent() == entity_2 || entity_2->parent() == entity_1)
        return true;

    if(!LPhys::Collision_Resolution__Rigid_Body_2D::resolve(_id))
        return false;

    entity_1->on_collision(_id);
    entity_2->on_collision(_id);

    return true;
}
