#ifndef ENTITY_H
#define ENTITY_H

#include <Object_System/Rigid_Body_2D.h>


namespace GSSG
{

    class Entity : public LEti::Rigid_Body_2D
    {
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

    };

}


#endif // ENTITY_H
