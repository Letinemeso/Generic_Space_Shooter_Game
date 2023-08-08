#include <Game_World/Physics_Module__Space_Ship.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Physics_Module__Space_Ship, LPhys::Rigid_Body_2D)
FIELDS_END



LPhys::Physical_Model_2D* Physics_Module__Space_Ship::M_create_physical_model() const
{
    return new Physical_Model__Space_Ship;
}



void Physics_Module__Space_Ship::set_block_indices(const unsigned int* _xs, const unsigned int* _ys)
{
    Physical_Model__Space_Ship* model = (Physical_Model__Space_Ship*)get_physical_model();

    model->set_block_indices(_xs, _ys);
}
