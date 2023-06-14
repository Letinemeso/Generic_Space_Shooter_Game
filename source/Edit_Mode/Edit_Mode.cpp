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
    Block::Size arrays_sizes;

    unsigned int occupied_cells = 0;
    for(unsigned int x=0; x<m_grid->width(); ++x)
    {
        for(unsigned int y=0; y<m_grid->height(); ++y)
        {
            const Block& material = *m_grid->get_cell(x, y).material;

            if(material.get_id() == m_grid->no_material_id())
                continue;

            ++occupied_cells;
            arrays_sizes += material.get_size();
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

    float coords_scale = 1.0f / (float)(m_grid->width() > m_grid->height() ? m_grid->width() : m_grid->height());
    float single_block_mass_scale = 1.0f / (float)(m_grid->width() * m_grid->height());

    Block::Size offsets;
    for(unsigned int x=0; x<m_grid->width(); ++x)
    {
        for(unsigned int y=0; y<m_grid->height(); ++y)
        {
            const Grid::Cell& cell = m_grid->get_cell(x, y);

            const Block& block = *cell.material;

            if(block.get_id() == m_grid->no_material_id())
                continue;

            glm::vec3 coords_stride(coords_scale * (float)x, coords_scale * (float)y, 0.0f);

            block.copy_coords(coords, offsets.coords, coords_stride, cell.rotation_angle, coords_scale);
            block.copy_colors(colors, offsets.colors);
            block.copy_texture_coords(t_coords, offsets.texture_coords);
            block.copy_phys_coords(phys_coords, offsets.phys_coords, coords_stride, cell.rotation_angle, coords_scale);
            block.copy_collision_permissions(collision_permissions, offsets.collision_permissions);
            block.copy_masses(masses, offsets.masses, single_block_mass_scale);

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
    glm::vec3 grid_cell_size = m_grid->get_cell_size();
    grid_cell_size.z = 0.0f;
    glm::vec3 grid_size = m_grid->get_size();
    grid_size.z = 0.0f;
    glm::vec3 window_size(LEti::Window_Controller::get_window_data().width, LEti::Window_Controller::get_window_data().height, 0.0f);

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



void Edit_Mode::update()
{
    m_cursor_pos = m_camera->convert_window_coords({LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f});

    m_grid->update();
}
