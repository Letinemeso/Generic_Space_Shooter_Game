#include <Edit_Mode/Grid.h>

using namespace GSSG;


Grid::Grid()
{

}

Grid::~Grid()
{
    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete m_cells[i];
    delete m_collision_detector;
}



void Grid::set_position(const glm::vec3 &_position)
{
    L_ASSERT(m_cells.size() > 0);

    for(unsigned int i=0; i<m_cells.size(); ++i)
        (*m_cells[i]).set_pos(_position);
}

void Grid::construct()
{
    glm::vec3 cell_size = m_cell_stub->scale;
    glm::vec3 total_size_halved(cell_size.x * m_width * 0.5f, cell_size.y * m_height * 0.5f, 0.0f);

    m_collision_detector->unregister_all_objects();

    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete m_cells[i];
    m_cells.clear();

    m_cells.resize(m_width * m_height);

    for(unsigned int x=0; x<m_width; ++x)
    {
        float horizontal_stride = cell_size.x * (float)x;

        for(unsigned int y=0; y<m_height; ++y)
        {
            float vertical_stride = cell_size.y * (float)y;
            glm::vec3 total_stride = glm::vec3(horizontal_stride, vertical_stride, 0.0f) - total_size_halved;
            total_stride.x /= cell_size.x - 0.5f;
            total_stride.y /= cell_size.y - 0.5f;

            LEti::Object_2D* cell = (LEti::Object_2D*)m_cell_stub->construct();
            cell->draw_module()->move_raw(total_stride);
            cell->physics_module()->move_raw(total_stride);
            m_cells.push(cell);
        }
    }

    for(unsigned int i=0; i<m_cells.size(); ++i)
        m_collision_detector->register_object(m_cells[i]);
}



void Grid::update()
{
    for(LDS::Vector<LEti::Object_2D*>::Iterator it = m_cells.iterator(); !it.end_reached(); ++it)
    {
        (*it)->update_previous_state();
        (*it)->update();
    }

    m_collision_detector->update();

    for(std::list<LEti::Physical_Model_2D::Intersection_Data>::const_iterator it = m_collision_detector->get_collisions__points().begin();
        it != m_collision_detector->get_collisions__points().end();
        ++it)
    {
        m_on_cell_pressed_func((LEti::Object_2D*)it->first);
    }

    for(LDS::Vector<LEti::Object_2D*>::Iterator it = m_cells.iterator(); !it.end_reached(); ++it)
        m_renderer->draw(*(*it)->draw_module());
}
