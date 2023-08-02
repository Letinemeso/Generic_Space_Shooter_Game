#include <Edit_Mode/Edit_Mode.h>

using namespace GSSG;


Edit_Mode::Edit_Mode()
{

}

Edit_Mode::~Edit_Mode()
{

}



void Edit_Mode::M_reconstruct_player_stub()
{
    m_player_stub->structure = m_grid->structure();
}



void Edit_Mode::on_activate()
{
    m_grid->set_structure(m_player_stub->structure);

    m_grid->construct();

    glm::vec3 grid_cell_size = m_grid->get_cell_size();
    grid_cell_size.z = 0.0f;
    glm::vec3 grid_size = m_grid->get_size();
    grid_size.z = 0.0f;
    glm::vec3 window_size(LR::Window_Controller::get_window_data().width, LR::Window_Controller::get_window_data().height, 0.0f);

    glm::vec3 grid_pos = m_camera->position();

    grid_pos.x -= window_size.x * 0.5f;
    grid_pos.y += window_size.y * 0.5f;

    grid_pos.x += grid_cell_size.x * 1.5f;
    grid_pos.y -= grid_cell_size.y * 2.5f;

    grid_pos.x += grid_size.x * 0.5f;
    grid_pos.y -= grid_size.y * 0.5f;

//    m_grid->set_position(m_camera->position());
    m_grid->set_position(grid_pos);
}

void Edit_Mode::on_deactivate()
{
    m_player_controller->forcefuly_kill_player();

    M_reconstruct_player_stub();
}



bool Edit_Mode::can_be_deactivated() const
{
    for(unsigned int x = 0; x < m_grid->structure().width(); ++x)
        for(unsigned int y = 0; y < m_grid->structure().height(); ++y)
            if(m_grid->structure().block(x, y).material != nullptr)
                return true;
    return false;
}



void Edit_Mode::update()
{
    m_cursor_pos = m_camera->convert_window_coords({LR::Window_Controller::get_cursor_position().x, LR::Window_Controller::get_cursor_position().y, 0.0f});

    m_grid->update();
}
