#ifndef PHYSICAL_MODEL__SPACE_SHIP_H
#define PHYSICAL_MODEL__SPACE_SHIP_H

#include <Physics/Physical_Models/Rigid_Body_Physical_Model_2D.h>

#include <Game_World/Polygon__Space_Ship.h>


namespace GSSG
{

    class Physical_Model__Space_Ship : public LEti::Rigid_Body_Physical_Model_2D
    {
    private:
        unsigned int* m_block_indices_x = nullptr;
        unsigned int* m_block_indices_y = nullptr;

    public:
        ~Physical_Model__Space_Ship();

    private:
        LEti::Polygon_Holder_Base* M_create_polygons_holder() const override;

    public:
        void set_block_indices(const unsigned int* _xs, const unsigned int* _ys);

    };

}


#endif // PHYSICAL_MODEL__SPACE_SHIP_H
