#include <Game_World/Visual_Effect.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Visual_Effect_Stub, LEti::Object_2D_Stub)

ADD_CHILD("draw_module", *draw_module)

FIELDS_END



LV::Variable_Base* Visual_Effect_Stub::M_construct_product() const
{
    return new Visual_Effect;
}

void Visual_Effect_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    LEti::Object_2D_Stub::M_init_constructed_product(_product);

    Visual_Effect* product = (Visual_Effect*)_product;

    LR::Draw_Module__Animation* dm = (LR::Draw_Module__Animation*)draw_module->construct();

    product->add_module(dm);

    product->should_be_destroyed = [dm]()
    {
        return dm->repetitions() >= 1;
    };
}





INIT_FIELDS(GSSG::Visual_Effect, LEti::Object_2D)
FIELDS_END
