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



void Grid::set_position(const glm::vec3 &_position)
{
    L_ASSERT(m_cells.size() > 0);

//    for(unsigned int i=0; i<m_cells.size(); ++i)
//        ((Cell&)m_cells[i]).object->set_pos(_position);

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
//            total_stride.x /= cell_size.x;
//            total_stride.y /= cell_size.y;

            Cell cell;
            cell.object = (LEti::Object_2D*)m_cell_stub->construct();
//            cell.object->draw_module()->move_raw(total_stride);
//            cell.object->physics_module()->move_raw(total_stride);
            cell.object->set_pos(total_stride);
            cell.index_x = x;
            cell.index_y = y;
            m_cells.push(cell);
            m_cells_map.insert(cell.object, &((Cell&)m_cells[m_cells.size() - 1]));
        }
    }

    for(unsigned int i=0; i<m_cells.size(); ++i)
        m_collision_detector->register_object(((Cell&)m_cells[i]).object);
}

void Grid::set_cell_visual_data(Cell &_cell,
                                const float *_coords, unsigned int _coords_amount,
                                const float *_colors, unsigned int _colors_amount,
                                const float *_texture_coords, unsigned int _texture_coords_amount)
{
    LEti::Default_Draw_Module_2D* draw_module = _cell.object->draw_module();

    draw_module->init_vertices(_coords, _coords_amount);
    draw_module->init_colors(_colors, _colors_amount);
    draw_module->init_texture(draw_module->texture().get_picture(), _texture_coords, _texture_coords_amount);
}

void Grid::reset_cell(Cell &_cell)
{
    const LEti::Default_Draw_Module_2D_Stub* stub = (LEti::Default_Draw_Module_2D_Stub*)m_cell_stub->draw_module;

    set_cell_visual_data(_cell,
                         stub->coords, stub->coords_count,
                         stub->colors, stub->colors_count,
                         stub->tcoords, stub->tcoords_count);
}



void Grid::update()
{
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
        m_renderer->draw(*((Cell&)(*it)).object->draw_module());
}
