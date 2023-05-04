#ifndef GRID_H
#define GRID_H

#include <Data_Structures/Map.h>
#include <Stuff/Function_Wrapper.h>

#include <Renderer.h>
#include <Object_System/Object_2D.h>
#include <Physics/Collision_Detector_2D.h>

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
        };

    private:
        const LEti::Renderer* m_renderer = nullptr;
        LEti::Collision_Detector_2D* m_collision_detector = nullptr;

        const LEti::Object_2D_Stub* m_cell_stub = nullptr;

    private:
        unsigned int m_width = 0, m_height = 0;
        LDS::Vector<Cell> m_cells;
        LDS::Map<const LEti::Object_2D*, Cell*> m_cells_map;
        LEti::Object_2D* m_cell_preview = nullptr;
        LST::Function<void(Cell&)> m_on_cell_pressed_func;

    public:
        Grid();
        ~Grid();

    public:
        inline void set_collision_detector(LEti::Collision_Detector_2D* _ptr) { delete m_collision_detector; m_collision_detector = _ptr; }  //  takes ownership
        inline void set_renderer(const LEti::Renderer* _ptr) { m_renderer = _ptr; }
        inline void set_cell_stub(const LEti::Object_2D_Stub* _stub) { m_cell_stub = _stub; }
        inline void set_on_cell_pressed_func(const LST::Function<void(Cell&)>& _func) { m_on_cell_pressed_func = _func; }

        inline void set_size_parameters(unsigned int _width, unsigned int _height) { m_width = _width; m_height = _height; }

    private:
        void M_set_object_visual_data(LEti::Object_2D* _object,
                                      const float* _coords, unsigned int _coords_amount,
                                      const float* _colors, unsigned int _colors_amount,
                                      const float* _texture_coords, unsigned int _texture_coords_amount);

    public:
        void set_position(const glm::vec3& _position);
        void construct();
        void set_cell_visual_data(Cell& _cell,
                                  const float* _coords, unsigned int _coords_amount,
                                  const float* _colors, unsigned int _colors_amount,
                                  const float* _texture_coords, unsigned int _texture_coords_amount);
        void reset_cell(Cell& _cell);
        void set_preview_visual_data(const float* _coords, unsigned int _coords_amount,
                                     const float* _colors, unsigned int _colors_amount,
                                     const float* _texture_coords, unsigned int _texture_coords_amount);

    public:
        void update();

    };

}

#endif // GRID_H
