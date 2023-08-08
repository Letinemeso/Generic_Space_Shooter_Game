#include <Game_World/Collision_Resolution__Entity.h>

using namespace GSSG;


bool Collision_Resolution__Entity::resolve(const LPhys::Intersection_Data &_id)
{
    Entity* maybe_entity_1 = LV::cast_variable<Entity>(_id.first->associated_object());
    Entity* maybe_entity_2 = LV::cast_variable<Entity>((LEti::Object_2D*)_id.second->associated_object());

    if(!maybe_entity_1 || !maybe_entity_2)
        return false;

    if(maybe_entity_1->parent() == maybe_entity_2 || maybe_entity_2->parent() == maybe_entity_1)
        return true;

    if(!LPhys::Collision_Resolution__Rigid_Body_2D::resolve(_id))
        return false;

    maybe_entity_1->on_collision(_id);
    maybe_entity_2->on_collision(_id);

    return true;
}
