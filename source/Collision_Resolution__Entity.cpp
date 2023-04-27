#include <Collision_Resolution__Entity.h>

using namespace GSSG;


bool Collision_Resolution__Entity::resolve(const LEti::Intersection_Data &_id)
{
    Entity* maybe_entity_1 = LV::cast_variable<Entity>((LEti::Object_2D*)_id.first);
    Entity* maybe_entity_2 = LV::cast_variable<Entity>((LEti::Object_2D*)_id.second);

    if(!maybe_entity_1 || !maybe_entity_2)
        return false;

    if(maybe_entity_1->parent() == maybe_entity_2 || maybe_entity_2->parent() == maybe_entity_1)
        return true;

    int health_1 = maybe_entity_1->health();
    int health_2 = maybe_entity_2->health();

    maybe_entity_1->set_health(health_1 - health_2);
    maybe_entity_2->set_health(health_2 - health_1);

    if(!LEti::Collision_Resolution__Rigid_Body_2D::resolve(_id))
        return false;

    maybe_entity_1->on_collision(maybe_entity_2);
    maybe_entity_2->on_collision(maybe_entity_1);

    return true;
}
