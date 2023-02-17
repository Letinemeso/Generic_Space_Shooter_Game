#include <Entity_Manager.h>

using namespace GSSG;


Entity_Manager::Entity_Manager()
{

}

Entity_Manager::~Entity_Manager()
{
    for(LDS::List<Entity*>::Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        delete *it;
}



void Entity_Manager::add_entity(Entity *_entity)
{
    L_ASSERT(_entity);

    m_entities.push_back(_entity);
    m_collision_detector->register_object(_entity);
}



void Entity_Manager::M_delete_entity(Entity *_entity)
{
    L_ASSERT(_entity);

    LDS::List<Entity*>::Iterator entity_as_iterator;
    for(LDS::List<Entity*>::Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
    {
        if((*it) == _entity)
            entity_as_iterator = it;

        if((*it)->parent() == _entity)
            (*it)->set_parent(nullptr);
    }

    L_ASSERT(entity_as_iterator.is_ok());

    m_collision_detector->unregister_object(_entity);
    delete _entity;
    m_entities.erase(entity_as_iterator);
}



void Entity_Manager::update_entities_prev_state() const
{
    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->update_previous_state();
}

void Entity_Manager::update_entities(float _ratio) const
{
    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->update(_ratio);
}


void Entity_Manager::remove_dead_entities()
{
    L_ASSERT(m_collision_detector);

    LDS::List<Entity*> entities_to_delete;

    for(LDS::List<Entity*>::Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
    {
        if((*it)->should_be_destroyed() || ( LEti::Math::vector_length(m_camera->position() - (*it)->get_pos()) > m_max_distance_from_view_pos ) )
            entities_to_delete.push_back(*it);
    }

    for(LDS::List<Entity*>::Iterator it = entities_to_delete.begin(); !it.end_reached() && it.is_ok(); ++it)
        M_delete_entity(*it);
}


void Entity_Manager::draw_entities() const
{
    L_ASSERT(m_renderer);

    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        m_renderer->draw(*(*it)->draw_module());
}
