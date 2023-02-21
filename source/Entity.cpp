#include <Entity.h>

using namespace GSSG;

INIT_FIELDS(GSSG::Entity, LEti::Rigid_Body_2D);
FIELDS_END;


Entity::Entity()
{

}

Entity::~Entity()
{

}



void Entity::on_delete_other_entity(const Entity *_entity_to_delete)
{
    if(m_parent == _entity_to_delete)
        m_parent = nullptr;
}
