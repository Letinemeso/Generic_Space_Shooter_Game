#ifndef ENEMY_ENTITY_GENERATOR_H
#define ENEMY_ENTITY_GENERATOR_H

#include <Math_Stuff.h>
#include <Timer.h>

#include <Entity_Manager.h>
#include <Entity.h>


namespace GSSG
{

    class Enemy_Entity_Generator
    {
    private:
        Entity_Manager* m_entity_manager = nullptr;
        const LEti::Object_2D_Stub* m_entity_stub = nullptr;

        glm::vec3 m_spawn_position{0.0f, 0.0f, 0.0f};

        float m_spawn_frequency = 1.0f;
        LEti::Timer m_timer;

    public:
        Enemy_Entity_Generator();
        ~Enemy_Entity_Generator();

    public:
        inline void set_entity_stub(const LEti::Object_2D_Stub* _entity_stub) { m_entity_stub = _entity_stub; }
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }
        inline void set_spawn_frequency(float _spawn_frequency) { m_spawn_frequency = _spawn_frequency; }
        inline void set_spawn_position(const glm::vec3& _position) { m_spawn_position = _position; }

    public:
        void update();

    public:
        void spawn_enemy() const;

    };

}


#endif // ENEMY_ENTITY_GENERATOR_H
