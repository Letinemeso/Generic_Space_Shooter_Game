#ifndef PLAYER_H
#define PLAYER_H


#include <Data_Structures/List.h>

#include <Event_Controller.h>

#include <Enemy_Generator.h>
#include <Entity_Manager.h>
#include <Entity.h>
#include <Projectile.h>
#include <Space_Ship.h>


namespace GSSG
{

    class Player : public GSSG::Space_Ship
    {
    private:
        LEti::Camera_2D* m_camera = nullptr;

    public:
        Player();
        ~Player();

    public:
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }

    public:
        void apply_input() override;
        void update(float _ratio = 1.0f) override;

    };

}


#endif // PLAYER_H
