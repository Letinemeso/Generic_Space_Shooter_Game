#ifndef GRID_CELL_H
#define GRID_CELL_H

#include <Object_System/Object_2D.h>

#include <Modules/Physics_Module_2D.h>

#include <Edit_Mode/Block_Controller.h>


namespace GSSG
{

    class Grid_Cell_Stub : public LEti::Object_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        LPhys::Physics_Module_2D_Stub* physics_module = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        ~Grid_Cell_Stub();

    };

    class Grid_Cell : public LEti::Object_2D
    {
    public:
        DECLARE_VARIABLE;

    private:
        LPhys::Physics_Module_2D* m_physics_module = nullptr;

    private:
        unsigned int m_index_x = 0, m_index_y = 0;
        const Block* m_material = nullptr;

    public:
        Grid_Cell();
        ~Grid_Cell();

    public:
        inline void set_physics_module(LPhys::Physics_Module_2D* _ptr) { m_physics_module = _ptr; }
        inline LPhys::Physics_Module_2D* physics_module() { return m_physics_module; }
        inline const LPhys::Physics_Module_2D* physics_module() const { return m_physics_module; }

    public:
        inline void set_indices(unsigned int _x, unsigned int _y) { m_index_x = _x; m_index_y = _y; }
        inline void set_material(const Block* _material) { m_material = _material; }

    public:
        inline unsigned int index_x() const { return m_index_x; }
        inline unsigned int index_y() const { return m_index_y; }
        inline const Block* material() const { return m_material; }

    };

}

#endif // GRID_CELL_H
