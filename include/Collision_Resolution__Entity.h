#ifndef COLLISION_RESOLUTION__ENTITY_H
#define COLLISION_RESOLUTION__ENTITY_H

#include <Physics/Collision_Resolution__Rigid_Body_2D.h>

#include <Entity.h>


namespace GSSG
{

    class Collision_Resolution__Entity : public LEti::Collision_Resolution__Rigid_Body_2D
    {
    public:
        bool resolve(const LEti::Intersection_Data &_id) override;

    };

}


#endif // COLLISION_RESOLUTION__ENTITY_H
