#include <Game_World/Entity_Physics_Module.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Entity_Physics_Module_Stub, LPhys::Rigid_Body_2D__Stub)
FIELDS_END



LV::Variable_Base* Entity_Physics_Module_Stub::M_construct_product() const
{
    return new Entity_Physics_Module;
}






INIT_FIELDS(GSSG::Entity_Physics_Module, LPhys::Rigid_Body_2D)
FIELDS_END
