#include <Game_World/Entity_Manager.h>

#include <Modules/Rigid_Body_2D.h>  //  TODO: this is not right but ill fix this later

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

    m_entities.push_front(_entity);
    m_collision_detector->register_object(_entity->physics_module());
}



void Entity_Manager::M_notify_other_entities_about_death(Entity *_entity)
{
    L_ASSERT(_entity);

    for(LDS::List<Entity*>::Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->on_other_entity_death(_entity);
}



void Entity_Manager::update_entities_prev_state() const
{
    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->update_previous_state();
}

void Entity_Manager::update_entities(float _dt) const
{
    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->update(_dt);
}

void Entity_Manager::apply_entities_input() const
{
    for(LDS::List<Entity*>::Const_Iterator it = m_entities.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->apply_input();
}


void Entity_Manager::remove_dead_entities()
{
    L_ASSERT(m_collision_detector);

    if(m_entities.size() == 0)
        return;

    LDS::List<Entity*>::Iterator it = m_entities.begin();

    while(true)
    {
        if((*it)->should_be_destroyed() || ( LEti::Math::vector_length(m_camera->position() - (*it)->current_state().position()) > m_max_distance_from_view_pos ) )
        {
            LDS::List<Entity*>::Iterator next = it;
            if(!it.end_reached())
                ++next;

            Entity* entity = *it;
            m_entities.erase(it);

            M_notify_other_entities_about_death(entity);
            m_collision_detector->unregister_object(entity->physics_module());
            entity->on_death();
            delete entity;

            it = next;
        }

        if(it.end_reached() || !it.is_ok() || m_entities.size() == 0)
            break;

        ++it;
    }
}
