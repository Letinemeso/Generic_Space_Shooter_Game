#include <Edit_Mode/Grid.h>

using namespace GSSG;


Grid::Grid()
{

}

Grid::~Grid()
{
    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete ((Cell&)m_cells[i]).object;
    delete m_collision_detector;
}



void Grid::M_set_object_visual_data(LEti::Object_2D* _object,
                                const float *_coords, unsigned int _coords_amount,
                                const float *_colors, unsigned int _colors_amount,
                                const float *_texture_coords, unsigned int _texture_coords_amount)
{
    LEti::Default_Draw_Module_2D* draw_module = _object->draw_module();

    draw_module->init_vertices(_coords, _coords_amount);
    draw_module->init_colors(_colors, _colors_amount);
    draw_module->init_texture(draw_module->texture().get_picture(), _texture_coords, _texture_coords_amount);
}



void Grid::set_position(const glm::vec3 &_position)
{
    L_ASSERT(m_cells.size() > 0);

    glm::vec3 cell_size = m_cell_stub->scale;
    glm::vec3 total_size_halved(cell_size.x * m_width * 0.5f, cell_size.y * m_height * 0.5f, 0.0f);
    for(unsigned int x=0; x<m_width; ++x)
    {
        float horizontal_stride = cell_size.x * (float)x;

        for(unsigned int y=0; y<m_height; ++y)
        {
            unsigned int index = x * m_height + y;

            float vertical_stride = cell_size.y * (float)y;
            glm::vec3 total_stride = glm::vec3(horizontal_stride, vertical_stride, 0.0f) - total_size_halved + _position;

            Cell& cell = m_cells[index];
            cell.object->set_pos(total_stride);
        }
    }

    glm::vec3 total_size_stride = total_size_halved;
    total_size_stride.y *= -1.0f;
    glm::vec3 cell_size_stride = cell_size;
    cell_size_stride.x += cell_size.x;
    cell_size_stride.y *= -1.0f;
    cell_size_stride.z = 0.0f;

    m_cell_preview->set_pos( _position - total_size_stride - cell_size_stride );
}

void Grid::construct()
{
    m_collision_detector->unregister_all_objects();

    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete ((Cell&)m_cells[i]).object;
    m_cells.clear();
    m_cells_map.clear();

    m_cells.resize(m_width * m_height);

    glm::vec3 cell_size = m_cell_stub->scale;
    glm::vec3 total_size_halved(cell_size.x * m_width * 0.5f, cell_size.y * m_height * 0.5f, 0.0f);

    for(unsigned int x=0; x<m_width; ++x)
    {
        float horizontal_stride = cell_size.x * (float)x;

        for(unsigned int y=0; y<m_height; ++y)
        {
            float vertical_stride = cell_size.y * (float)y;
            glm::vec3 total_stride = glm::vec3(horizontal_stride, vertical_stride, 0.0f) - total_size_halved;

            Cell cell;
            cell.object = (LEti::Object_2D*)m_cell_stub->construct();
            cell.object->set_pos(total_stride);
            cell.index_x = x;
            cell.index_y = y;
            m_cells.push(cell);
            m_cells_map.insert(cell.object, &((Cell&)m_cells[m_cells.size() - 1]));
        }
    }

    for(unsigned int i=0; i<m_cells.size(); ++i)
        m_collision_detector->register_object(((Cell&)m_cells[i]).object);

    delete m_cell_preview;
    m_cell_preview = (LEti::Object_2D*)m_cell_stub->construct();
}

void Grid::set_cell_visual_data(Cell& _cell,
                                const float *_coords, unsigned int _coords_amount,
                                const float *_colors, unsigned int _colors_amount,
                                const float *_texture_coords, unsigned int _texture_coords_amount)
{
    M_set_object_visual_data(_cell.object,
                             _coords, _coords_amount,
                             _colors, _colors_amount,
                             _texture_coords, _texture_coords_amount);
}

void Grid::reset_cell(Cell& _cell)
{
    const LEti::Default_Draw_Module_2D_Stub* stub = (LEti::Default_Draw_Module_2D_Stub*)m_cell_stub->draw_module;

    M_set_object_visual_data(_cell.object,
                             stub->coords, stub->coords_count,
                             stub->colors, stub->colors_count,
                             stub->tcoords, stub->tcoords_count);
}

void Grid::set_preview_visual_data(const float *_coords, unsigned int _coords_amount,
                                   const float *_colors, unsigned int _colors_amount,
                                   const float *_texture_coords, unsigned int _texture_coords_amount)
{
    M_set_object_visual_data(m_cell_preview,
                             _coords, _coords_amount,
                             _colors, _colors_amount,
                             _texture_coords, _texture_coords_amount);
}



void Grid::update()
{
    m_cell_preview->update_previous_state();
    m_cell_preview->update();

    for(LDS::Vector<Cell>::Iterator it = m_cells.iterator(); !it.end_reached(); ++it)
    {
        ((Cell&)(*it)).object->update_previous_state();
        ((Cell&)(*it)).object->update();
    }

    m_collision_detector->update();

    for(LDS::List<LEti::Intersection_Data>::Const_Iterator it = m_collision_detector->get_collisions__points().begin(); !it.end_reached(); ++it)
    {
        LDS::Map<const LEti::Object_2D*, Cell*>::Iterator cell_map_it = m_cells_map.find(it->first);
        Cell* cell = *cell_map_it;
        m_on_cell_pressed_func(*cell);
    }

    for(LDS::Vector<Cell>::Iterator it = m_cells.iterator(); !it.end_reached(); ++it)
        ((Cell&)(*it)).object->draw(*m_renderer);
    m_cell_preview->draw(*m_renderer);
}
