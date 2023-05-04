#ifndef GRID_H
#define GRID_H

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Renderer.h>
#include <Object_System/Object_2D.h>
#include <Physics/Collision_Detector_2D.h>

#include <Edit_Mode/Block_Controller.h>


namespace GSSG
{

    class Grid final
    {
    public:
        struct Cell
        {
            LEti::Object_2D* object = nullptr;
            unsigned int index_x = 0, index_y = 0;
            unsigned int material_id = 0;
            float rotation_angle = 0.0f;
        };

    private:
        const LEti::Renderer* m_renderer = nullptr;
        LEti::Collision_Detector_2D* m_collision_detector = nullptr;

        const LEti::Object_2D_Stub* m_cell_stub = nullptr;

        const Block_Controller* m_block_controller = nullptr;

    private:
        unsigned int m_width = 0, m_height = 0;
        LDS::Vector<Cell> m_cells;
        LDS::Map<const LEti::Object_2D*, Cell*> m_cells_map;
        LEti::Object_2D* m_cell_preview = nullptr;

    private:
        unsigned int m_no_material_id = 0;
        unsigned int m_material_id = 0;

    public:
        Grid();
        ~Grid();

    public:
        inline void set_collision_detector(LEti::Collision_Detector_2D* _ptr) { m_collision_detector = _ptr; }
        inline void set_renderer(const LEti::Renderer* _ptr) { m_renderer = _ptr; }
        inline void set_cell_stub(const LEti::Object_2D_Stub* _stub) { m_cell_stub = _stub; }
        inline void set_block_controller(const Block_Controller* _ptr) { m_block_controller = _ptr; m_material_id = m_block_controller->get_block_ids()[0]; m_no_material_id = m_material_id; }

    public:
        inline unsigned int width() const { return m_width; }
        inline unsigned int height() const { return m_height; }
        inline unsigned int no_material_id() const { return m_no_material_id; }

    private:
        void M_on_cell_pressed(Cell& _cell);
        void M_set_object_visual_data(LEti::Object_2D* _object, const Block& _block);

    public:
        void set_position(const glm::vec3& _position);
        void construct(unsigned int _width, unsigned int _height);
        void set_preview_visual_data(const Block& _block);

    public:
        const Cell& get_cell(unsigned int _x, unsigned int _y) const;

    private:
        void M_apply_input();

    public:
        void update();

    };

}

#endif // GRID_H
