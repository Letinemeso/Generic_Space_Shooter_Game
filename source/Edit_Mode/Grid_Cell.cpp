#include <Edit_Mode/Grid_Cell.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Grid_Cell_Stub, LEti::Object_2D_Stub)

ADD_CHILD("draw_module", *draw_module)
ADD_CHILD("physics_module", *physics_module)

FIELDS_END


LV::Variable_Base* Grid_Cell_Stub::M_construct_product() const
{
    return new Grid_Cell;
}

void Grid_Cell_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    LEti::Object_2D_Stub::M_init_constructed_product(_product);

    Grid_Cell* result = (Grid_Cell*)_product;

    LR::Default_Draw_Module_2D* dm = (LR::Default_Draw_Module_2D*)draw_module->construct();
    Grid_Cell_Physics_Module* pm = (Grid_Cell_Physics_Module*)physics_module->construct();

    result->set_physics_module(pm);
    pm->set_owner(result);

    result->set_visual_data = [dm](const float* _coords, unsigned int _coords_amount, const float* _colors, unsigned int _colors_amount, const float* _t_coords, unsigned int _t_coords_amount)
    {
        dm->init_vertices(_coords, _coords_amount);
        dm->init_colors(_colors, _colors_amount);
        dm->init_texture(dm->texture().get_picture(), _t_coords, _t_coords_amount);
    };

    result->add_module(pm);
    result->add_module(dm);
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

}



INIT_FIELDS(GSSG::Grid_Cell_Physics_Module, LPhys::Physics_Module_2D)
FIELDS_END



INIT_FIELDS(GSSG::Grid_Cell_Physics_Module_Stub, LPhys::Physics_Module_2D_Stub)
FIELDS_END



LV::Variable_Base* Grid_Cell_Physics_Module_Stub::M_construct_product() const
{
    return new Grid_Cell_Physics_Module;
}
