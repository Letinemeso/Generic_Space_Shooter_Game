#include <Game_World/Polygon__Space_Ship.h>

using namespace GSSG;


Polygon__Space_Ship::Polygon__Space_Ship()
    : LEti::Rigid_Body_Polygon()
{

}

Polygon__Space_Ship::Polygon__Space_Ship(const Polygon__Space_Ship& _other)
    : LEti::Rigid_Body_Polygon(_other)
{
    m_block_index_x = _other.m_block_index_x;
    m_block_index_y = _other.m_block_index_y;
}

Polygon__Space_Ship::~Polygon__Space_Ship()
{

}


void Polygon__Space_Ship::setup(const Polygon& _other)
{
    Rigid_Body_Polygon::setup(_other);

    const Polygon__Space_Ship& other = (const Polygon__Space_Ship&)_other;

    m_block_index_x = other.m_block_index_x;
    m_block_index_y = other.m_block_index_y;
}
