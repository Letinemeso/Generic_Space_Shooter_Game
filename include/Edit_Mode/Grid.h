#ifndef GRID_H
#define GRID_H

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Renderer/Renderer.h>
#include <Object_System/Object_2D.h>
#include <Collision_Detection/Collision_Detector_2D.h>

#include <Edit_Mode/Block_Controller.h>
#include <Edit_Mode/Grid_Cell.h>


namespace GSSG
{

    class Grid final
    {
    private:
        const LR::Renderer* m_renderer = nullptr;
        LPhys::Collision_Detector_2D* m_collision_detector = nullptr;

        const Grid_Cell_Stub* m_cell_stub = nullptr;

        const Block_Controller* m_block_controller = nullptr;

    private:
        Space_Ship_Structure m_structure;

        unsigned int m_width = 0, m_height = 0;
        LDS::Vector<Grid_Cell*> m_cells;
        Grid_Cell* m_cell_preview = nullptr;

    private:
        const Block* m_no_material = nullptr;
        const Block* m_material = nullptr;

    public:
        Grid();
        ~Grid();

    public:
        inline void set_collision_detector(LPhys::Collision_Detector_2D* _ptr) { m_collision_detector = _ptr; }
        inline void set_renderer(const LR::Renderer* _ptr) { m_renderer = _ptr; }
        inline void set_cell_stub(const Grid_Cell_Stub* _stub) { m_cell_stub = _stub; }
        inline void set_block_controller(const Block_Controller* _ptr) { m_block_controller = _ptr; m_material = &m_block_controller->get_block(0); m_no_material = m_material; }

    public:
        inline unsigned int width() const { return m_width; }
        inline unsigned int height() const { return m_height; }
        inline unsigned int no_material_id() const { return m_no_material->get_id(); }
        inline const Space_Ship_Structure& structure() const { return m_structure; }

    private:
        const Grid_Cell& M_get_cell_with_coordinates(unsigned int x, unsigned int y) const;
        Grid_Cell& M_get_cell_with_coordinates(unsigned int x, unsigned int y);

        void M_reset_cell(Grid_Cell& _cell);
        void M_reset_cells();

        void M_update_cells();

        void M_on_cell_pressed(Grid_Cell& _cell);
        void M_set_cell_visual_data(Grid_Cell& _object, const Block& _block);

    public:
        void set_position(const glm::vec3& _position);
        void construct();
        void set_preview_visual_data(const Block& _block);
        void set_structure(const Space_Ship_Structure& _structure);

    public:
        const Grid_Cell& get_cell(unsigned int _x, unsigned int _y) const;

        glm::vec3 get_cell_size() const;
        glm::vec3 get_size() const;

    private:
        void M_apply_input();

    public:
        void update();

    };

}

#endif // GRID_H
