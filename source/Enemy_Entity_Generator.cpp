#include <Enemy_Entity_Generator.h>

using namespace GSSG;


Enemy_Entity_Generator::Enemy_Entity_Generator()
{

}

Enemy_Entity_Generator::~Enemy_Entity_Generator()
{

}



void Enemy_Entity_Generator::update()
{
    L_ASSERT(m_entity_stub);

    m_timer.update();
    if(!m_timer.is_active())
    {
        spawn_enemy();
        m_timer.start(m_spawn_frequency);
    }
}



void Enemy_Entity_Generator::spawn_enemy() const
{
    L_ASSERT(m_entity_stub);

    glm::vec3 position = m_spawn_position;
    position.x += LEti::Math::random_number(100, 400) * (LEti::Math::random_number(0, 2) == 0 ? -1.0f : 1.0f);
    position.y += LEti::Math::random_number(100, 400) * (LEti::Math::random_number(0, 2) == 0 ? -1.0f : 1.0f);

    float random_scale = LEti::Math::random_number(1, 6);

    Entity* entity = new Entity;
    entity->init(*m_entity_stub);
    entity->set_pos(position);
    entity->set_health(random_scale);
    entity->set_scale(5.0f * (float)random_scale);
    entity->set_mass(3.0f * (float)random_scale);
    entity->set_rotation_angle(LEti::Math::DOUBLE_PI / 360.0f * (float)LEti::Math::random_number(0, 360));

    for(unsigned int i=0; i<3; ++i)
    {
        float random_color = (float)LEti::Math::random_number(1, 11) / 10.0f;
        for(unsigned int c=i; c<entity->draw_module()->colors().size(); c += 4)
            entity->draw_module()->colors()[c] = random_color;
    }

    m_entity_manager->add_entity(entity);
}
