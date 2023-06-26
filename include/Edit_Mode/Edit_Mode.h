#ifndef EDIT_MODE_H
#define EDIT_MODE_H

#include <Camera_2D.h>
#include <Physics/Collision_Detector_2D.h>
#include <Physics/Space_Hasher_2D.h>
#include <Physics/Dynamic_Narrow_CD.h>
#include <Physics/SAT_Narrowest_CD.h>

#include <Game_Logic.h>
#include <Edit_Mode/Block_Controller.h>
#include <Edit_Mode/Grid.h>
#include <Game_World/Player_Controller.h>
#include <Game_World/Player.h>

namespace GSSG
{

    class Edit_Mode : public Game_Logic
    {
    private:
        const LEti::Camera_2D* m_camera = nullptr;
        glm::vec3 m_cursor_pos{0.0f, 0.0f, 0.0f};

    private:
        Player_Controller* m_player_controller = nullptr;
        Player_Stub* m_player_stub = nullptr;
        Block_Controller* m_block_controller = nullptr;
        Grid* m_grid = nullptr;

    public:
        Edit_Mode();
        ~Edit_Mode();

    public:
        void on_activate() override;
        void on_deactivate() override;

    public:
        bool can_be_deactivated() const override;

    private:
        void M_reconstruct_player_stub();

    public:
        inline void set_camera(const LEti::Camera_2D* _ptr) { m_camera = _ptr; }
        inline void set_player_controller(Player_Controller* _ptr) { m_player_controller = _ptr; }
        inline void set_player_stub(Player_Stub* _ptr) { m_player_stub = _ptr; }
        inline void set_grid(Grid* _ptr) { m_grid = _ptr; }
        inline void set_block_controller(Block_Controller* _ptr) { m_block_controller = _ptr; }

    public:
        void update() override;

    };

}

#endif // EDIT_MODE_H
