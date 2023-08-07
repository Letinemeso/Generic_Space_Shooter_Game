#include <Game_World/Physical_Model__Space_Ship.h>

using namespace GSSG;


Physical_Model__Space_Ship::~Physical_Model__Space_Ship()
{

}



LPhys::Polygon_Holder_Base* Physical_Model__Space_Ship::M_create_polygons_holder() const
{
    return new LPhys::Polygon_Holder<Polygon__Space_Ship>;
}



void Physical_Model__Space_Ship::set_block_indices(const unsigned int* _xs, const unsigned int* _ys)
{
    for(unsigned int i = 0; i < get_polygons_count(); ++i)
    {
        Polygon__Space_Ship* polygon = (Polygon__Space_Ship*)get_polygon(i);
        polygon->set_block_index(_xs[i], _ys[i]);
    }
}
