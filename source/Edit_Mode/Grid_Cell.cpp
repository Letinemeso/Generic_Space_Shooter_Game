#include <Edit_Mode/Grid_Cell.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Grid_Cell_Stub, LEti::Object_2D_Stub)

ADD_CHILD("physics_module", *physics_module)

FIELDS_END


LV::Variable_Base* Grid_Cell_Stub::M_construct_product() const
{
    return new Entity;
}

void Grid_Cell_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    LEti::Object_2D_Stub::M_init_constructed_product(_product);

    Grid_Cell* result = (Grid_Cell*)_product;

    Grid_Cell_Physics_Module* pm = (Grid_Cell_Physics_Module*)physics_module->construct();

    result->add_module(pm);
    result->set_physics_module(pm);
    pm->set_owner(result);
}



Grid_Cell_Stub::~Grid_Cell_Stub()
{
    delete physics_module;
}



INIT_FIELDS(GSSG::Grid_Cell, LEti::Object_2D)
FIELDS_END


Grid_Cell::Grid_Cell()
{

}

Grid_Cell::~Grid_Cell()
{
    delete m_physics_module;
}



INIT_FIELDS(GSSG::Grid_Cell_Physics_Module, LPhys::Physics_Module_2D)
FIELDS_END



INIT_FIELDS(GSSG::Grid_Cell_Physics_Module_Stub, LPhys::Physics_Module_2D_Stub)
FIELDS_END



LV::Variable_Base* Grid_Cell_Physics_Module_Stub::M_construct_product() const
{
    return new Grid_Cell_Physics_Module;
}
