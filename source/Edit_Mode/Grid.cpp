#include <Edit_Mode/Grid.h>

using namespace GSSG;


Grid::Grid()
{

}

Grid::~Grid()
{
    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete ((Cell&)m_cells[i]).object;
}



const Grid::Cell& Grid::M_get_cell_with_coordinates(unsigned int x, unsigned int y) const
{
    unsigned int index = x * m_height + y;
    return m_cells[index];
}

Grid::Cell& Grid::M_get_cell_with_coordinates(unsigned int x, unsigned int y)
{
    unsigned int index = x * m_height + y;
    return m_cells[index];
}

bool Grid::M_cabin_is_placed() const
{
    for(unsigned int i = 0; i < m_cells.size(); ++i)
    {
        const Cell& cell = m_cells[i];

        if(LV::cast_variable<Cabin>(cell.material) != nullptr)
            return true;
    }

    return false;
}


unsigned int Grid::M_get_connection_permission_index(unsigned int _expected_index, float _rotation_angle) const
{
    float full_quarters = fabs(_rotation_angle / LEti::Math::HALF_PI);
    unsigned int quarters = LEti::Math::float_to_int(full_quarters) % 4;

    if(_rotation_angle < 0.0f)
        quarters = abs(((int)quarters - 4) % 4);

    unsigned int result = _expected_index + 4 - quarters;
    if(result >= 4)
        result -= 4;

    return result;
}

bool Grid::M_block_can_be_placed(const Cell& _cell) const
{
    if(m_material == m_no_material || (LV::cast_variable<Cabin>(m_material) != nullptr) )
        return true;

    const Block& block_data = *m_material;

    for(unsigned int i=0; i<4; ++i)
    {
        unsigned int resolution_index = M_get_connection_permission_index(i, m_cell_preview->get_rotation_angle());

        int x = (int)_cell.index_x;
        int y = (int)_cell.index_y;

        if(i == 0)
            ++x;
        else if(i == 1)
            ++y;
        else if(i == 2)
            --x;
        else if(i == 3)
            --y;

        if(x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height)
            continue;

        const Cell& nearby_cell = M_get_cell_with_coordinates(x, y);

        if(nearby_cell.material == m_no_material)
            continue;

        const Block& nearby_block_data = *nearby_cell.material;

        unsigned int expected_nearby_index = i + 2;
        if(expected_nearby_index >= 4)
            expected_nearby_index -= 4;
        unsigned int nearby_index = M_get_connection_permission_index(expected_nearby_index, nearby_cell.object->get_rotation_angle());

        if(block_data.get_connection_permissions()[resolution_index] && nearby_block_data.get_connection_permissions()[nearby_index])
            return true;
    }

    return false;
}


void Grid::M_update_cell_connections(const Cell& _cell)
{
    unsigned int index = _cell.index_x * m_height + _cell.index_y;

    auto update_neighbour = [this, index, &_cell](unsigned int _which)->void
    {
        const Cell& neighbour = m_cells[_which];
        bool should_be_connected = _cell.material != m_no_material && neighbour.material != m_no_material;

        unsigned int expected_connection_permission_index = 0;
        if(_cell.index_x < neighbour.index_x)
            expected_connection_permission_index = 0;
        else if(_cell.index_x > neighbour.index_x)
            expected_connection_permission_index = 2;
        else if(_cell.index_y < neighbour.index_y)
            expected_connection_permission_index = 1;
        else if(_cell.index_y > neighbour.index_y)
            expected_connection_permission_index = 3;

        unsigned int connection_permission_index = M_get_connection_permission_index(expected_connection_permission_index, _cell.object->get_rotation_angle());
        unsigned int neighbour_connection_permission_index = M_get_connection_permission_index((expected_connection_permission_index + 2) % 4, neighbour.object->get_rotation_angle());

        if(_cell.material->get_connection_permissions()[connection_permission_index] && neighbour.material->get_connection_permissions()[neighbour_connection_permission_index])\
            should_be_connected = true;
        else
            should_be_connected = false;

        if(should_be_connected)
        {
            if(m_graph.distance_if_linked(index, _which) == 0)
                m_graph.link_nodes(index, _which, 1, true);
        }
        else
        {
            if(m_graph.distance_if_linked(index, _which) > 0)
                m_graph.unlink_nodes(index, _which, true);
        }
    };

    if(_cell.index_x > 0)
        update_neighbour((_cell.index_x - 1) * m_height + _cell.index_y);
    if(_cell.index_x < m_width - 1)
        update_neighbour((_cell.index_x + 1) * m_height + _cell.index_y);
    if(_cell.index_y > 0)
        update_neighbour(_cell.index_x * m_height + _cell.index_y - 1);
    if(_cell.index_y < m_height - 1)
        update_neighbour(_cell.index_x * m_height + _cell.index_y + 1);
}


void Grid::M_reset_cell(Cell& _cell)
{
    _cell.material = m_no_material;
    M_set_object_visual_data(_cell.object, *m_no_material);
    _cell.rotation_angle = 0.0f;
    _cell.object->set_rotation_angle(_cell.rotation_angle);

    _cell.object->update(0.0f);
}

void Grid::M_reset_cells()
{
    for(unsigned int i = 0; i < m_cells.size(); ++i)
    {
        Cell& cell = m_cells[i];
        M_reset_cell(cell);
    }
}

void Grid::M_reset_not_connected_blocks()
{
    for(unsigned int i = 0; i < m_cells.size(); ++i)
    {
        Cell& cell = m_cells[i];

        if(cell.material == m_no_material || i == m_cabin_cell_index)
            continue;

        m_block_connection_check.process(i, m_cabin_cell_index);
        if(m_block_connection_check.path_result().size() > 0)
            continue;

        M_reset_cell(cell);
        M_update_cell_connections(cell);
    }
}

void Grid::M_on_cell_pressed(Cell& _cell)
{
    if(LV::cast_variable<Cabin>(m_material) != nullptr)
    {
        M_reset_cells();
        m_cabin_cell_index = _cell.index_x * m_height + _cell.index_y;
    }
    else if(!M_cabin_is_placed())
        return;
    else if(LV::cast_variable<Cabin>(_cell.material) != nullptr)
        return;

    if(!M_block_can_be_placed(_cell))
        return;

    _cell.material = m_material;
    M_set_object_visual_data(_cell.object, *m_material);
    _cell.rotation_angle = m_cell_preview->get_rotation_angle();
    _cell.object->set_rotation_angle(_cell.rotation_angle);

    _cell.object->update(0.0f);

    M_update_cell_connections(_cell);

    M_reset_not_connected_blocks();
}

void Grid::M_set_object_visual_data(LEti::Object_2D* _object, const Block& _block)
{
    LEti::Default_Draw_Module_2D* draw_module = _object->draw_module();

    draw_module->init_vertices(_block.get_coords(), _block.get_size().coords);
    draw_module->init_colors(_block.get_colors(), _block.get_size().colors);
    draw_module->init_texture(draw_module->texture().get_picture(), _block.get_texture_coords(), _block.get_size().texture_coords);
}



void Grid::set_position(const glm::vec3 &_position)
{
    L_ASSERT(m_cells.size() > 0);

    glm::vec3 cell_size = m_cell_stub->scale;
    glm::vec3 total_size_halved = get_size() * 0.5f;
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
    cell_size_stride.x = 0.0f;
    cell_size_stride.y *= -1.0f;
    cell_size_stride.z = 0.0f;

    m_cell_preview->set_pos( _position - total_size_stride - cell_size_stride );
}

void Grid::construct(unsigned int _width, unsigned int _height)
{
    m_width = _width;
    m_height = _height;

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

    m_cabin_cell_index = m_cells.size();

    m_graph.clear();
    m_graph.allocate_nodes(m_cells.size());
    m_block_connection_check.set_graph(&m_graph);
    m_block_connection_check.set_distance_calculation_func([this](unsigned int _first, unsigned int _second)
    {
        const Cell& first = m_cells[_first];
        const Cell& second = m_cells[_second];
        return abs((int)first.index_x - (int)second.index_x) + abs((int)first.index_y - (int)second.index_y);
    });
}

void Grid::set_preview_visual_data(const Block& _block)
{
    M_set_object_visual_data(m_cell_preview, _block);
}



const Grid::Cell& Grid::get_cell(unsigned int _x, unsigned int _y) const
{
    unsigned int index = _x * m_height + _y;
    return m_cells[index];
}


glm::vec3 Grid::get_cell_size() const
{
    return m_cell_preview->get_scale();
}

glm::vec3 Grid::get_size() const
{
    glm::vec3 size = m_cell_preview->get_scale();
    size.x *= (float)m_width;
    size.y *= (float)m_height;
    return size;
}



void Grid::M_apply_input()
{
    unsigned int material_before_input = m_material->get_id();

    for(unsigned int i=GLFW_KEY_1; i<=GLFW_KEY_9; ++i)
    {
        if(!LEti::Event_Controller::key_was_pressed(i))
            continue;

        unsigned int id = i - GLFW_KEY_0;

        if(!m_block_controller->block_exists(id))
            continue;

        m_material = &m_block_controller->get_block(id);
    }

    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_BACKSPACE))
        m_material = m_no_material;

    if(m_material->get_id() != material_before_input)
    {
        const Block& block = *m_material;
        set_preview_visual_data(block);
    }

    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_E))
        m_cell_preview->set_rotation_angle(m_cell_preview->get_rotation_angle() - LEti::Math::HALF_PI);
    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_Q))
        m_cell_preview->set_rotation_angle(m_cell_preview->get_rotation_angle() + LEti::Math::HALF_PI);

    if(!LEti::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_1))
        return;

    for(LDS::List<LEti::Intersection_Data>::Const_Iterator it = m_collision_detector->get_collisions__points().begin(); !it.end_reached(); ++it)
    {
        LDS::Map<const LEti::Object_2D*, Cell*>::Iterator cell_map_it = m_cells_map.find(it->first);
        Cell* cell = *cell_map_it;
        M_on_cell_pressed(*cell);
    }
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


    M_apply_input();


    for(LDS::Vector<Cell>::Iterator it = m_cells.iterator(); !it.end_reached(); ++it)
        ((Cell&)(*it)).object->draw(*m_renderer);
    m_cell_preview->draw(*m_renderer);
}
