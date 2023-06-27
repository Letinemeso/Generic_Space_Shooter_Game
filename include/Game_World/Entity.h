#ifndef ENTITY_H
#define ENTITY_H

#include <Object_System/Rigid_Body_2D_Stub.h>
#include <Physics/Intersection_Data.h>

#include <Effects_Controller.h>


namespace GSSG
{

    class Entity : public LEti::Object_2D
    {
    public:
        DECLARE_VARIABLE;

    protected:
        int m_health = 0.0f;

        const Entity* m_parent = nullptr;

    protected:
        Effects_Controller* m_effects_controller = nullptr;
        const LEti::Object_2D_Stub* m_on_death_effect = nullptr;

    public:
        Entity();
        ~Entity();

    protected:
        inline LEti::Physics_Module__Rigid_Body_2D* M_get_physics_module() { return (LEti::Physics_Module__Rigid_Body_2D*)physics_module(); }

    public:
        inline void inject_effects_controller(Effects_Controller* _effects_controller) { m_effects_controller = _effects_controller; }

        inline void set_on_death_effect(const LEti::Object_2D_Stub* _on_death_effect) { m_on_death_effect = _on_death_effect; }
        inline void set_parent(const Entity* _parent) { m_parent = _parent; }
        inline const Entity* parent() const { return m_parent; }

        inline void set_health(int _health) { m_health = _health; }

    public:
        inline int health() const { return m_health; }
        inline virtual bool should_be_destroyed() const { return m_health < 1; }

    public:
        virtual void apply_input() { }
        virtual void on_collision(const LEti::Intersection_Data& /*_id*/) { }
        virtual void on_other_entity_death(const Entity* _entity_to_delete);
        virtual void on_death();

    };

    class Entity_Stub : public LEti::Rigid_Body_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        Effects_Controller* effects_controller = nullptr;
        const LEti::Object_2D_Stub* on_death_effect = nullptr;

    public:
        int health = 1;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}


#endif // ENTITY_H
