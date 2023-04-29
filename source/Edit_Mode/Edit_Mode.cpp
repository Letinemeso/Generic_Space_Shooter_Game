#include <Edit_Mode/Edit_Mode.h>

using namespace GSSG;


Edit_Mode::Edit_Mode()
{
    m_materials = new int*[m_width];
    for(unsigned int i=0; i<m_width; ++i)
    {
        m_materials[i] = new int[m_height];
        for(unsigned int j=0; j<m_height; ++j)
            m_materials[i][j] = -1;
    }
}

Edit_Mode::~Edit_Mode()
{
    delete m_grid;

    for(unsigned int i=0; i<m_width; ++i)
        delete[] m_materials[i];
    delete[] m_materials;
}



void Edit_Mode::M_reconstruct_player_stub()
{
    unsigned int occupied_cells = 0;
    for(unsigned int x=0; x<m_width; ++x)
    {
        for(unsigned int y=0; y<m_height; ++y)
        {
            if(m_materials[x][y] != -1)
                ++occupied_cells;
        }
    }

    if(occupied_cells == 0)
        return;

    unsigned int coords_count = occupied_cells * 18;
    unsigned int colors_count = occupied_cells * 24;
    unsigned int t_coords_count = occupied_cells * 12;
    unsigned int masses_count = occupied_cells * 2;

    float* coords = new float[coords_count];
    float* colors = new float[colors_count];
    float* t_coords = new float[t_coords_count];
    bool* collision_permissions = new bool[occupied_cells * 6];
    float* masses = new float[masses_count];

    float raw_coord_stride_unscaled = 0.5f / (float)m_width;
    float polygon_mass = 1.0f / (m_width * m_height * 2.0f);

    unsigned int cells_reconstructed = 0;
    for(unsigned int x=0; x<m_width; ++x)
    {
        for(unsigned int y=0; y<m_height; ++y)
        {
            if(m_materials[x][y] == -1)
                continue;

            unsigned int coords_stride = cells_reconstructed * 18;
            unsigned int colors_stride = cells_reconstructed * 24;
            unsigned int t_coords_stride = cells_reconstructed * 12;
            unsigned int collision_permissions_stride = cells_reconstructed * 6;
            unsigned int masses_stride = cells_reconstructed * 2;

            coords[coords_stride + 0]  = raw_coord_stride_unscaled;     coords[coords_stride + 1]  = raw_coord_stride_unscaled;     coords[coords_stride + 2]  = 0.0f;
            coords[coords_stride + 3]  = -raw_coord_stride_unscaled;    coords[coords_stride + 4]  = raw_coord_stride_unscaled;     coords[coords_stride + 5]  = 0.0f;
            coords[coords_stride + 6]  = -raw_coord_stride_unscaled;    coords[coords_stride + 7]  = -raw_coord_stride_unscaled;    coords[coords_stride + 8]  = 0.0f;
            coords[coords_stride + 9]  = raw_coord_stride_unscaled;     coords[coords_stride + 10] = raw_coord_stride_unscaled;     coords[coords_stride + 11] = 0.0f;
            coords[coords_stride + 12] = -raw_coord_stride_unscaled;    coords[coords_stride + 13] = -raw_coord_stride_unscaled;    coords[coords_stride + 14] = 0.0f;
            coords[coords_stride + 15] = raw_coord_stride_unscaled;     coords[coords_stride + 16] = -raw_coord_stride_unscaled;    coords[coords_stride + 17] = 0.0f;

            for(unsigned int i=coords_stride; i<coords_stride + 18; i+= 3)
            {
                coords[i] += raw_coord_stride_unscaled * 2.0f * (float)x;
                coords[i + 1] += raw_coord_stride_unscaled * 2.0f * (float)y;
            }

            collision_permissions[collision_permissions_stride + 0] = true;  collision_permissions[collision_permissions_stride + 1] = true; collision_permissions[collision_permissions_stride + 2] = false;
            collision_permissions[collision_permissions_stride + 3] = false; collision_permissions[collision_permissions_stride + 4] = true; collision_permissions[collision_permissions_stride + 5] = true;


            for(unsigned int i=colors_stride; i<colors_stride + 24; ++i)
                colors[i] = 1.0f;

            t_coords[t_coords_stride + 0] = 60; t_coords[t_coords_stride + 1] = 60;
            t_coords[t_coords_stride + 2] = 0;  t_coords[t_coords_stride + 3] = 60;
            t_coords[t_coords_stride + 4] = 0;  t_coords[t_coords_stride + 5] = 0;
            t_coords[t_coords_stride + 6] = 60; t_coords[t_coords_stride + 7] = 60;
            t_coords[t_coords_stride + 8] = 0;  t_coords[t_coords_stride + 9] = 0;
            t_coords[t_coords_stride + 10] = 60; t_coords[t_coords_stride + 11] = 0;

            masses[masses_stride] = polygon_mass;
            masses[masses_stride + 1] = polygon_mass;

            ++cells_reconstructed;
        }
    }

    LEti::Default_Draw_Module_2D_Stub* dms = (LEti::Default_Draw_Module_2D_Stub*)m_player_stub->draw_module;
    LEti::Physics_Module__Rigid_Body_2D__Stub* pms = (LEti::Physics_Module__Rigid_Body_2D__Stub*)m_player_stub->physics_module;

    delete[] dms->coords;
    dms->coords = coords;
    dms->coords_count = coords_count;
    delete[] dms->colors;
    dms->colors = colors;
    dms->colors_count = colors_count;
    delete[] dms->tcoords;
    dms->tcoords = t_coords;
    dms->tcoords_count = t_coords_count;

    float* phys_coords = new float[coords_count];
    for(unsigned int i=0; i<coords_count; ++i)
        phys_coords[i] = coords[i];

    delete[] pms->coords;
    pms->coords = phys_coords;
    pms->coords_count = coords_count;
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



void Edit_Mode::M_on_cell_chosen(Grid::Cell &_cell)
{
    const LEti::Picture* picture = nullptr;

    int material = m_chosen_material;

    if(LEti::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_1))
        material = m_chosen_material;
    else if(LEti::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_2))
        material = -1;
    else
        return;

    if(material == m_chosen_material)
        picture = LEti::Picture_Manager::get_picture("white_texture");
    else if(material == -1)
        picture = LEti::Picture_Manager::get_picture("grid_cell_texture");

    LEti::Default_Draw_Module_2D* dm = _cell.object->draw_module();
    dm->set_texture(picture);

    m_materials[_cell.index_x][_cell.index_y] = material;
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

    m_grid->update();
}
