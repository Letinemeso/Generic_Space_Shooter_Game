#include <Edit_Mode/Edit_Mode.h>

using namespace GSSG;


Edit_Mode::Edit_Mode()
{
    m_materials = new unsigned int*[m_width];
    for(unsigned int i=0; i<m_width; ++i)
    {
        m_materials[i] = new unsigned int[m_height];
        for(unsigned int j=0; j<m_height; ++j)
            m_materials[i][j] = 0;
    }
}

Edit_Mode::~Edit_Mode()
{
    delete m_grid;
    delete m_block_controller;

    for(unsigned int i=0; i<m_width; ++i)
        delete[] m_materials[i];
    delete[] m_materials;
}



void Edit_Mode::M_reconstruct_player_stub()
{
    Block::Size arrays_sizes;

    unsigned int occupied_cells = 0;
    for(unsigned int x=0; x<m_width; ++x)
    {
        for(unsigned int y=0; y<m_height; ++y)
        {
            if(m_materials[x][y] == 0)
                continue;

            ++occupied_cells;
            arrays_sizes += m_block_controller->get_block(m_materials[x][y]).get_size();
        }
    }

    if(occupied_cells == 0)
        return;

    float* coords = new float[arrays_sizes.coords];
    float* colors = new float[arrays_sizes.colors];
    float* t_coords = new float[arrays_sizes.texture_coords];
    float* phys_coords = new float[arrays_sizes.phys_coords];
    bool* collision_permissions = new bool[arrays_sizes.collision_permissions];
    float* masses = new float[arrays_sizes.masses];

    float coords_scale = 1.0f / (float)(m_width > m_height ? m_width : m_height);
    float single_block_mass_scale = 1.0f / (float)(m_width * m_height);

    Block::Size offsets;

    for(unsigned int x=0; x<m_width; ++x)
    {
        for(unsigned int y=0; y<m_height; ++y)
        {
            if(m_materials[x][y] == 0)
                continue;

            const Block& block = m_block_controller->get_block(m_materials[x][y]);

            block.copy_coords(coords, offsets.coords);
            block.copy_colors(colors, offsets.colors);
            block.copy_texture_coords(t_coords, offsets.texture_coords);
            block.copy_phys_coords(phys_coords, offsets.phys_coords);
            block.copy_collision_permissions(collision_permissions, offsets.collision_permissions);
            block.copy_masses(masses, offsets.masses);

            for(unsigned int i=offsets.coords; i<offsets.coords + block.get_size().coords; ++i)
                coords[i] *= coords_scale;
            for(unsigned int i=offsets.phys_coords; i<offsets.phys_coords + block.get_size().phys_coords; ++i)
                phys_coords[i] *= coords_scale;
            for(unsigned int i=offsets.masses; i<offsets.masses + block.get_size().masses; ++i)
                masses[i] *= single_block_mass_scale;

            for(unsigned int i=offsets.coords; i<offsets.coords + block.get_size().coords; i+= 3)
            {
                coords[i] += coords_scale * (float)x;
                coords[i + 1] += coords_scale * (float)y;
            }
            for(unsigned int i=offsets.phys_coords; i<offsets.phys_coords + block.get_size().phys_coords; i+= 3)
            {
                phys_coords[i] += coords_scale * (float)x;
                phys_coords[i + 1] += coords_scale * (float)y;
            }

            offsets += block.get_size();
        }
    }

    LEti::Default_Draw_Module_2D_Stub* dms = (LEti::Default_Draw_Module_2D_Stub*)m_player_stub->draw_module;
    LEti::Physics_Module__Rigid_Body_2D__Stub* pms = (LEti::Physics_Module__Rigid_Body_2D__Stub*)m_player_stub->physics_module;

    delete[] dms->coords;
    dms->coords = coords;
    dms->coords_count = arrays_sizes.coords;
    delete[] dms->colors;
    dms->colors = colors;
    dms->colors_count = arrays_sizes.colors;
    delete[] dms->tcoords;
    dms->tcoords = t_coords;
    dms->tcoords_count = arrays_sizes.texture_coords;

    for(unsigned int i=0; i<arrays_sizes.coords; ++i)
        phys_coords[i] = coords[i];

    delete[] pms->coords;
    pms->coords = phys_coords;
    pms->coords_count = arrays_sizes.phys_coords;
    delete[] pms->collision_permissions;
    pms->collision_permissions = collision_permissions;
    delete[] pms->masses;
    pms->masses = masses;
}



void Edit_Mode::on_activate()
{
    m_grid->set_position(m_camera->position());
}

void Edit_Mode::on_deactivate()
{
    m_player_controller->forcefuly_kill_player();

    M_reconstruct_player_stub();
}



void Edit_Mode::M_on_cell_chosen(Grid::Cell& _cell)
{
    if(!LEti::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_1))
        return;

    m_materials[_cell.index_x][_cell.index_y] = m_chosen_material;

    const Block& block = m_block_controller->get_block(m_chosen_material);

    m_grid->set_cell_visual_data(_cell,
                                 block.get_coords(), block.get_size().coords,
                                 block.get_colors(), block.get_size().colors,
                                 block.get_texture_coords(), block.get_size().texture_coords);
}



void Edit_Mode::set_grid(Grid *_ptr)
{
    delete m_grid;
    m_grid = _ptr;

    LEti::Collision_Detector_2D* grid_collision_detector = new LEti::Collision_Detector_2D;
    grid_collision_detector->set_broad_phase(new LEti::Space_Hasher_2D, 10);
    grid_collision_detector->set_narrow_phase(new LEti::Dynamic_Narrow_CD, 10);
    grid_collision_detector->set_narrowest_phase(new LEti::SAT_Narrowest_CD);
    grid_collision_detector->register_point(&m_cursor_pos);

    m_grid->set_collision_detector(grid_collision_detector);
    m_grid->set_size_parameters(m_width, m_height);
    m_grid->construct();
    m_grid->set_on_cell_pressed_func( [this](GSSG::Grid::Cell& _cell) { M_on_cell_chosen(_cell); } );
}



void Edit_Mode::update()
{
    m_cursor_pos = m_camera->convert_window_coords({LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f});

    unsigned int material_before_input = m_chosen_material;

    for(unsigned int i=GLFW_KEY_1; i<=GLFW_KEY_9; ++i)
    {
        if(!LEti::Event_Controller::key_was_pressed(i))
            continue;

        unsigned int id = i - GLFW_KEY_0;

        if(!m_block_controller->block_exists(id))
            continue;

        m_chosen_material = id;
    }

    if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_BACKSPACE))
        m_chosen_material = 0;

    if(m_chosen_material != material_before_input)
    {
        const Block& block = m_block_controller->get_block(m_chosen_material);
        m_grid->set_preview_visual_data(block.get_coords(), block.get_size().coords,
                                        block.get_colors(), block.get_size().colors,
                                        block.get_texture_coords(), block.get_size().texture_coords);
    }

    m_grid->update();
}
