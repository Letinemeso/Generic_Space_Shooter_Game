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

    float* coords = new float[occupied_cells * 18];
    float* colors = new float[occupied_cells * 24];
    float* t_coords = new float[occupied_cells * 12];

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

            coords[coords_stride + 0]  = 0.5f; coords[coords_stride + 1]  = 0.5f; coords[coords_stride + 2]  = 0.0f;
            coords[coords_stride + 9]  = -0.5f; coords[coords_stride + 4]  = 0.5f; coords[coords_stride + 5]  = 0.0f;
            coords[coords_stride + 3]  = -0.5f; coords[coords_stride + 7]  = -0.5f; coords[coords_stride + 8]  = 0.0f;
            coords[coords_stride + 6]  = 0.5f; coords[coords_stride + 10] = 0.5f; coords[coords_stride + 11] = 0.0f;
            coords[coords_stride + 12] = -0.5f; coords[coords_stride + 13] = -0.5f; coords[coords_stride + 14] = 0.0f;
            coords[coords_stride + 15] = 0.5f; coords[coords_stride + 16] = -0.5f; coords[coords_stride + 17] = 0.0f;

        }
    }

}



void Edit_Mode::on_activate()
{
    m_grid->set_position(m_camera->position());
}

void Edit_Mode::on_deactivate()
{
    m_player_controller->forcefuly_kill_player();

//    unsigned int
}



void Edit_Mode::M_on_cell_chosen(Grid::Cell &_cell)
{
    const LEti::Picture* picture = nullptr;

    int material = m_chosen_material;

    if(LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_1))
        material = m_chosen_material;
    else if(LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_2))
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
