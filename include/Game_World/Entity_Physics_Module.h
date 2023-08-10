#ifndef ENTITY_PHYSICS_MODULE_H
#define ENTITY_PHYSICS_MODULE_H

#include <Modules/Rigid_Body_2D.h>


namespace GSSG
{

    class Entity;

    class Entity_Physics_Module_Stub : public LPhys::Rigid_Body_2D__Stub
    {
    public:
        DECLARE_VARIABLE;

    protected:
        LV::Variable_Base* M_construct_product() const override;

    };

    class Entity_Physics_Module : public LPhys::Rigid_Body_2D
    {
    public:
        DECLARE_VARIABLE;

    private:
        Entity* m_owner = nullptr;

    public:
        inline void set_owner(Entity* _owner) { m_owner = _owner; }
        inline Entity* owner() const { return m_owner; }
    };

}

#endif // ENTITY_PHYSICS_MODULE_H
