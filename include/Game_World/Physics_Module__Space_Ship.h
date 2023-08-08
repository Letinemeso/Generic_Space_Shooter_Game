#ifndef PHYSICS_MODULE__SPACE_SHIP_H
#define PHYSICS_MODULE__SPACE_SHIP_H

#include <Modules/Rigid_Body_2D.h>

#include <Game_World/Physical_Model__Space_Ship.h>


namespace GSSG
{

    class Physics_Module__Space_Ship : public LPhys::Rigid_Body_2D
    {
    public:
        DECLARE_VARIABLE;

    private:
        LPhys::Physical_Model_2D* M_create_physical_model() const override;

    public:
        void set_block_indices(const unsigned int* _xs, const unsigned int* _ys);

    };

}


#endif // PHYSICS_MODULE__SPACE_SHIP_H
