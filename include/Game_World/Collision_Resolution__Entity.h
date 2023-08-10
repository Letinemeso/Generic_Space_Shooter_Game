#ifndef COLLISION_RESOLUTION__ENTITY_H
#define COLLISION_RESOLUTION__ENTITY_H

#include <Collision_Resolution/Collision_Resolution__Rigid_Body_2D.h>

#include <Game_World/Entity.h>
#include <Game_World/Entity_Physics_Module.h>


namespace GSSG
{

    class Collision_Resolution__Entity : public LPhys::Collision_Resolution__Rigid_Body_2D
    {
    public:
        bool resolve(const LPhys::Intersection_Data &_id) override;

    };

}


#endif // COLLISION_RESOLUTION__ENTITY_H
