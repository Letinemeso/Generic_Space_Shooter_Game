#include <Edit_Mode/Edit_Mode.h>

using namespace GSSG;


Edit_Mode::Edit_Mode()
{

}

Edit_Mode::~Edit_Mode()
{
    delete m_grid;
}



void Edit_Mode::on_activate()
{
    m_grid->set_position(m_camera->position());
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
    m_grid->set_size_parameters(11, 11);
    m_grid->construct();
}



void Edit_Mode::update()
{
    m_cursor_pos = m_camera->convert_window_coords({LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f});

    m_grid->update();
}
