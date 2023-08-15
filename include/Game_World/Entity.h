#ifndef ENTITY_H
#define ENTITY_H

#include <Collision_Detection/Intersection_Data.h>

#include <Game_World/Entity_Physics_Module.h>
#include <Game_World/Visual_Effect.h>
#include <Effects_Controller.h>


namespace GSSG
{

    class Entity_Physics_Module_Stub;

    class Entity_Stub : public LEti::Object_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        Entity_Physics_Module_Stub* physics_module = nullptr;
        LR::Default_Draw_Module_2D_Stub* draw_module = nullptr;

        Effects_Controller* effects_controller = nullptr;
        const Visual_Effect_Stub* on_death_effect = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        ~Entity_Stub();

    };

    class Entity : public LEti::Object_2D
    {
    public:
        DECLARE_VARIABLE;

    protected:
        const Entity* m_parent = nullptr;

    protected:
        Effects_Controller* m_effects_controller = nullptr;
        const Visual_Effect_Stub* m_on_death_effect = nullptr;

    protected:
        LPhys::Rigid_Body_2D* m_physics_module = nullptr;

    public:
        Entity();
        ~Entity();

    public:
        void update(float _dt) override;

    public:
        inline void set_physics_module(LPhys::Rigid_Body_2D* _ptr) { m_physics_module = _ptr; }
        inline LPhys::Rigid_Body_2D* physics_module() { return m_physics_module; }
        inline const LPhys::Rigid_Body_2D* physics_module() const { return m_physics_module; }

    public:
        inline void inject_effects_controller(Effects_Controller* _effects_controller) { m_effects_controller = _effects_controller; }

        inline void set_on_death_effect(const Visual_Effect_Stub* _on_death_effect) { m_on_death_effect = _on_death_effect; }
        inline void set_parent(const Entity* _parent) { m_parent = _parent; }
        inline const Entity* parent() const { return m_parent; }

    public:
        virtual bool should_be_destroyed() const { return true; }

    public:
        virtual void apply_input(float /*_dt*/) { } //  dt nor whole method are not needed here obviously, but ill care for them later
        virtual void on_collision(const LPhys::Intersection_Data& /*_id*/) { }
        virtual void on_other_entity_death(const Entity* _entity_to_delete);
        virtual void on_death();

    };

}


#endif // ENTITY_H
