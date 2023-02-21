#ifndef ENTITY_H
#define ENTITY_H

#include <Object_System/Rigid_Body_2D.h>


namespace GSSG
{

    class Entity : public LEti::Rigid_Body_2D
    {
    public:
        DECLARE_VARIABLE;

    protected:
        int m_health = 0.0f;

        const Entity* m_parent = nullptr;

    public:
        Entity();
        ~Entity();

    public:
        inline void set_parent(const Entity* _parent) { m_parent = _parent; }
        inline const Entity* parent() const { return m_parent; }

        inline void set_health(int _health) { m_health = _health; }

    public:
        inline int health() const { return m_health; }
        inline virtual bool should_be_destroyed() const { return m_health < 1; };

    public:
        virtual void apply_input() = 0;
        virtual void on_collision(Entity* /*_with*/) { }
        virtual void on_delete_other_entity(const Entity* _entity_to_delete);

    };

}


#endif // ENTITY_H
