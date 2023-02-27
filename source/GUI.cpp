#include <GUI.h>

using namespace GSSG;


GUI::GUI()
{
    m_camera.set_view_scale(1.0f);
    m_camera.set_position({LEti::Window_Controller::get_window_data().width / 2.0f, LEti::Window_Controller::get_window_data().height / 2.0f, 0.0f});

    m_renderer.set_camera(&m_camera);

    m_collision_detector.register_point(&m_cursor_position);
    m_collision_detector.set_broad_phase(new LEti::Space_Hasher_2D, 10);
    m_collision_detector.set_narrow_phase(new LEti::Dynamic_Narrow_CD, 10);
    m_collision_detector.set_narrowest_phase(new LEti::SAT_Narrowest_CD);
}

GUI::~GUI()
{

}



void GUI::add_object(LEti::Object_2D* _object, LST::Function_Wrapper<void, unsigned int>* _on_pressed, LST::Function_Wrapper<void, unsigned int>* _on_released)
{
    m_objects.push_back(GUI_Element_Data(_object, _on_pressed, _on_released));

    m_collision_detector.register_object(_object);
}

void GUI::remove_object(LEti::Object_2D* _object)
{
    m_collision_detector.unregister_object(_object);

    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if((*it).object == _object)
        {
            m_objects.erase(it);
            return;
        }
    }
}



void GUI::M_update_cursor_position()
{
    m_cursor_position = m_camera.convert_window_coords({LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f});
}

void GUI::M_process_mouse_down()
{
    if(!LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_1))
        return;

    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if(!(*it).on_pressed)
            continue;

        (*it).on_pressed->invoke(GLFW_MOUSE_BUTTON_1);  //  need to come up with some idea for passing different buttons here
    }
}

void GUI::M_process_mouse_up()
{
    if(!LEti::Event_Controller::mouse_button_was_released(GLFW_MOUSE_BUTTON_1))
        return;

    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if(!(*it).on_released)
            continue;

        (*it).on_released->invoke(GLFW_MOUSE_BUTTON_1);
    }
}



void GUI::update_prev_state()
{
    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
        (*it).object->update_previous_state();
}

void GUI::update(float _ratio)
{
    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
        (*it).object->update(_ratio);

    m_collision_detector.update();
}

void GUI::draw()
{
    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
        m_renderer.draw(*(*it).object->draw_module());
}