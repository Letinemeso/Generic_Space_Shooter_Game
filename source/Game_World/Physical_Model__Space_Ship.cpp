#include <Game_World/Physical_Model__Space_Ship.h>

using namespace GSSG;


Physical_Model__Space_Ship::~Physical_Model__Space_Ship()
{
    delete[] m_block_indices_x;
    delete[] m_block_indices_y;
}



LEti::Polygon_Holder_Base* Physical_Model__Space_Ship::M_create_polygons_holder() const
{
    return new LEti::Polygon_Holder<Polygon__Space_Ship>;
}



void Physical_Model__Space_Ship::set_block_indices(const unsigned int* _xs, const unsigned int* _ys)
{
    delete[] m_block_indices_x;
    delete[] m_block_indices_y;

    m_block_indices_x = new unsigned int[get_polygons_count()];
    m_block_indices_y = new unsigned int[get_polygons_count()];

    for(unsigned int i = 0; i < get_polygons_count(); ++i)
        m_block_indices_x[i] = _xs[i];
    for(unsigned int i = 0; i < get_polygons_count(); ++i)
        m_block_indices_y[i] = _ys[i];

    for(unsigned int i = 0; i < get_polygons_count(); ++i)
    {
        Polygon__Space_Ship* polygon = (Polygon__Space_Ship*)get_polygon(i);
        polygon->set_block_index(m_block_indices_x[i], m_block_indices_y[i]);
    }
}
