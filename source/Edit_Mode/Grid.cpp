#include <Edit_Mode/Grid.h>

using namespace GSSG;


Grid::Grid()
{

}

Grid::~Grid()
{
    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete m_cells[i];
}



const Grid_Cell& Grid::M_get_cell_with_coordinates(unsigned int x, unsigned int y) const
{
    unsigned int index = x * m_height + y;
    return *m_cells[index];
}

Grid_Cell& Grid::M_get_cell_with_coordinates(unsigned int x, unsigned int y)
{
    unsigned int index = x * m_height + y;
    return *m_cells[index];
}


void Grid::M_reset_cell(Grid_Cell& _cell)
{
    _cell.set_material(m_no_material);
    M_set_cell_visual_data(_cell, *m_no_material);
    _cell.current_state().set_rotation({0.0f, 0.0f, 0.0f});
}

void Grid::M_reset_cells()
{
    for(unsigned int i = 0; i < m_cells.size(); ++i)
        M_reset_cell(*m_cells[i]);
}


void Grid::M_update_cells()
{
    for(unsigned int x = 0; x < m_structure.width(); ++x)
    {
        for(unsigned int y = 0; y < m_structure.height(); ++y)
        {
            Grid_Cell& cell = M_get_cell_with_coordinates(x, y);
            const Space_Ship_Structure::Block_Data& block_data = m_structure.block(x, y);

            if(cell.material() == block_data.material && LEti::Math::floats_are_equal(cell.current_state().rotation().z, block_data.angle))
                continue;
            if(cell.material() == m_no_material && block_data.material == nullptr)
                continue;

            if(block_data.material)
                M_set_cell_visual_data(cell, *block_data.material);
            else
                M_set_cell_visual_data(cell, *m_no_material);

            cell.set_material(block_data.material);
            cell.current_state().set_rotation({0.0f, 0.0f, block_data.angle});

            cell.current_state().update_matrix();
        }
    }
}


void Grid::M_on_cell_pressed(Grid_Cell& _cell)
{
    Space_Ship_Structure::Block_Data block_data;
    block_data.angle = m_cell_preview->current_state().rotation().z;
    if(m_material != m_no_material)
        block_data.material = m_material;

    if(!m_structure.place_block(_cell.index_x(), _cell.index_y(), block_data))
        return;

    M_update_cells();
}

void Grid::M_set_cell_visual_data(Grid_Cell& _object, const Block& _block)
{
    _object.set_visual_data(_block.get_coords(), _block.get_size().coords, _block.get_colors(), _block.get_size().colors, _block.get_texture_coords(), _block.get_size().texture_coords);

//    LR::Default_Draw_Module_2D* draw_module = _object.draw_module();

//    draw_module->init_vertices(_block.get_coords(), _block.get_size().coords);
//    draw_module->init_colors(_block.get_colors(), _block.get_size().colors);
//    draw_module->init_texture(draw_module->texture().get_picture(), _block.get_texture_coords(), _block.get_size().texture_coords);
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

            Grid_Cell& cell = *m_cells[index];
            cell.current_state().set_position(total_stride);

            cell.current_state().update_matrix();
        }
    }

    glm::vec3 total_size_stride = total_size_halved;
    total_size_stride.y *= -1.0f;
    glm::vec3 cell_size_stride = cell_size;
    cell_size_stride.x += cell_size.x;
    cell_size_stride.x = 0.0f;
    cell_size_stride.y *= -1.0f;
    cell_size_stride.z = 0.0f;

    m_cell_preview->current_state().set_position( _position - total_size_stride - cell_size_stride );
    m_cell_preview->current_state().update_matrix();
}

void Grid::construct()
{
    L_ASSERT(m_structure.width() > 0 && m_structure.height() > 0);

    m_width = m_structure.width();
    m_height = m_structure.height();

    m_collision_detector->unregister_all_objects();

    for(unsigned int i=0; i<m_cells.size(); ++i)
        delete m_cells[i];
    m_cells.clear();

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

            Grid_Cell* cell = (Grid_Cell*)m_cell_stub->construct();
            cell->current_state().set_position(total_stride);
            cell->set_indices(x, y);
            m_cells.push(cell);
        }
    }

    for(unsigned int i=0; i<m_cells.size(); ++i)
        m_collision_detector->register_object((*m_cells[i]).physics_module());

    delete m_cell_preview;
    m_cell_preview = (Grid_Cell*)m_cell_stub->construct();
    M_set_cell_visual_data(*m_cell_preview, *m_no_material);

    m_material = m_no_material;

    M_update_cells();
}

void Grid::set_preview_visual_data(const Block& _block)
{
    M_set_cell_visual_data(*m_cell_preview, _block);
}

void Grid::set_structure(const Space_Ship_Structure &_structure)
{
    m_structure = _structure;
    construct();
}



const Grid_Cell& Grid::get_cell(unsigned int _x, unsigned int _y) const
{
    unsigned int index = _x * m_height + _y;
    return *m_cells[index];
}


glm::vec3 Grid::get_cell_size() const
{
    return m_cell_preview->current_state().scale();
}

glm::vec3 Grid::get_size() const
{
    glm::vec3 size = m_cell_preview->current_state().scale();
    size.x *= (float)m_width;
    size.y *= (float)m_height;
    return size;
}



void Grid::M_apply_input()
{
    unsigned int material_before_input = m_material->get_id();

    for(unsigned int i=GLFW_KEY_1; i<=GLFW_KEY_9; ++i)
    {
        if(!LR::Event_Controller::key_was_pressed(i))
            continue;

        unsigned int id = i - GLFW_KEY_0;

        if(!m_block_controller->block_exists(id))
            continue;

        m_material = &m_block_controller->get_block(id);
    }

    if(LR::Event_Controller::key_was_pressed(GLFW_KEY_BACKSPACE))
        m_material = m_no_material;

    if(m_material->get_id() != material_before_input)
    {
        const Block& block = *m_material;
        set_preview_visual_data(block);
    }

    if(LR::Event_Controller::key_was_pressed(GLFW_KEY_E))
        m_cell_preview->current_state().set_rotation({0.0f, 0.0f, m_cell_preview->current_state().rotation().z - LEti::Math::HALF_PI});
    if(LR::Event_Controller::key_was_pressed(GLFW_KEY_Q))
        m_cell_preview->current_state().set_rotation({0.0f, 0.0f, m_cell_preview->current_state().rotation().z + LEti::Math::HALF_PI});

    if(!LR::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_1))
        return;

    for(LDS::List<LPhys::Intersection_Data>::Const_Iterator it = m_collision_detector->get_collisions__points().begin(); !it.end_reached(); ++it)
        M_on_cell_pressed(*((Grid_Cell_Physics_Module*)it->first)->owner());
}



void Grid::update(float _dt)
{
    M_apply_input();

    m_cell_preview->update_previous_state();
    m_cell_preview->update(_dt);

    for(unsigned int i=0; i<m_cells.size(); ++i)
    {
        ((Grid_Cell&)(*m_cells[i])).update_previous_state();
        ((Grid_Cell&)(*m_cells[i])).update(_dt);
    }

    m_collision_detector->update();
}
