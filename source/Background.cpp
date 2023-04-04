#include <Background.h>

using namespace GSSG;


Background::Background()
{

}

Background::~Background()
{
    delete m_draw_module;
}



void Background::M_reconfigure()
{
    L_ASSERT(m_camera);
    L_ASSERT(m_picture);

    m_screen_size_scale = m_camera->view_scale();

    glm::vec2 screen_size;
    screen_size.x = LEti::Window_Controller::get_window_data().width * m_screen_size_scale;
    screen_size.y = LEti::Window_Controller::get_window_data().height * m_screen_size_scale;

    unsigned int full_images_amount_x = (unsigned int)screen_size.x / (unsigned int)m_picture->width();
    unsigned int full_images_amount_y = (unsigned int)screen_size.y / (unsigned int)m_picture->height();

    if(full_images_amount_x == 0)
        full_images_amount_x = 1;
    if(full_images_amount_y == 0)
        full_images_amount_y = 1;

    full_images_amount_x += 3;
    full_images_amount_y += 3;

    unsigned int total_images_amount = full_images_amount_x * full_images_amount_y;

    float* coords = new float[total_images_amount * 18];
    float* colors = new float[total_images_amount * 24];
    float* t_coords = new float[total_images_amount * 12];

    for(int x = 0; x < (int)full_images_amount_x; ++x)
    {
        for(int y = 0; y < (int)full_images_amount_y; ++y)
        {
            unsigned int v_index = (x + (y * full_images_amount_x)) * 18;
            unsigned int c_index = (x + (y * full_images_amount_x)) * 24;
            unsigned int t_index = (x + (y * full_images_amount_x)) * 12;

            coords[v_index + 0] =  m_picture->width() * (x + 1);
            coords[v_index + 15] = m_picture->width() * (x + 1);
            coords[v_index + 9] =  m_picture->width() * (x + 1);
            coords[v_index + 3] =  m_picture->width() * x;
            coords[v_index + 6] =  m_picture->width() * x;
            coords[v_index + 12] = m_picture->width() * x;

            coords[v_index + 1] =  m_picture->height() * (y + 1);
            coords[v_index + 4] =  m_picture->height() * (y + 1);
            coords[v_index + 10] = m_picture->height() * (y + 1);
            coords[v_index + 7] =  m_picture->height() * y;
            coords[v_index + 13] = m_picture->height() * y;
            coords[v_index + 16] = m_picture->height() * y;

            coords[v_index + 2] = 0.0f;
            coords[v_index + 5] = 0.0f;
            coords[v_index + 8] = 0.0f;
            coords[v_index + 11] = 0.0f;
            coords[v_index + 14] = 0.0f;
            coords[v_index + 17] = 0.0f;


            t_coords[t_index + 0] = 1;  t_coords[t_index + 1] = 1;
            t_coords[t_index + 2] = 0;  t_coords[t_index + 3] = 1;
            t_coords[t_index + 4] = 0;  t_coords[t_index + 5] = 0;
            t_coords[t_index + 6] = 1;  t_coords[t_index + 7] = 1;
            t_coords[t_index + 8] = 0;  t_coords[t_index + 9] = 1;
            t_coords[t_index + 10] = 1; t_coords[t_index + 11] = 0;

            for(unsigned int i=c_index; i < c_index + 24; ++i)
                colors[i] = 1.0f;
        }
    }

    for(unsigned int i=0; i<total_images_amount * 18; i += 3)
    {
        coords[i] -= LEti::Window_Controller::get_window_data().width * (m_screen_size_scale - 1.0f) * 0.5f + m_picture->width();
        coords[i + 1] -= LEti::Window_Controller::get_window_data().height * (m_screen_size_scale - 1.0f) * 0.5f + m_picture->height();
    }

    delete m_draw_module;
    m_draw_module = new LEti::Default_Draw_Module_2D;

    m_draw_module->init_vertices(coords, total_images_amount * 18);
    m_draw_module->init_colors(colors, total_images_amount * 24);
    m_draw_module->set_texture(m_picture);
    m_draw_module->set_texture_coords(t_coords, total_images_amount * 12);
//    m_draw_module->init_texture(m_picture, t_coords, total_images_amount * 12);

    delete[] coords;
    delete[] colors;
    delete[] t_coords;
}



void Background::update()
{
    if(!LEti::Math::floats_are_equal(m_camera->view_scale(), m_screen_size_scale))
        M_reconfigure();

    int full_stride_x = (int)m_camera->position().x / m_picture->width();
    int full_stride_y = (int)m_camera->position().y / m_picture->height();

    glm::vec2 stride;
    stride.x = (m_picture->width() * full_stride_x) - (LEti::Window_Controller::get_window_data().width / 2.0f);
    stride.y = (m_picture->height() * full_stride_y) - (LEti::Window_Controller::get_window_data().height / 2.0f);

    glm::mat4x4 default_matrix{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    glm::mat4x4 move_matrix = default_matrix;
    move_matrix[3][0] = stride.x;
    move_matrix[3][1] = stride.y;

    m_draw_module->update(move_matrix, default_matrix, default_matrix, 1.0f);
}
