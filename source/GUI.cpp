#include <GUI.h>

using namespace GSSG;


GUI::GUI()
{
    m_camera.set_view_scale(1.0f);
    m_camera.set_position({LR::Window_Controller::get_window_data().width / 2.0f, LR::Window_Controller::get_window_data().height / 2.0f, 0.0f});

    m_renderer.set_camera(&m_camera);

    LPhys::Space_Hasher_2D* cd_broad_phase = new LPhys::Space_Hasher_2D;
    cd_broad_phase->set_precision(10);
    m_collision_detector.set_broad_phase(cd_broad_phase);
    LPhys::Dynamic_Narrow_CD* cd_narrow_phase = new LPhys::Dynamic_Narrow_CD;
    cd_narrow_phase->set_precision(10);
    m_collision_detector.set_narrow_phase(cd_narrow_phase);
    m_collision_detector.set_narrowest_phase(new LPhys::SAT_Narrowest_CD);

    m_collision_detector.register_point(&m_cursor_position);
}

GUI::~GUI()
{

}



void GUI::add_object(LEti::Object_2D* _object, LPhys::Physics_Module_2D* _pm, const LST::Function<void(unsigned int)>& _on_pressed, const LST::Function<void(unsigned int)>& _on_released)
{
    m_objects.push_back(GUI_Element_Data(_object, _pm, _on_pressed, _on_released));

    if(_pm)
        m_collision_detector.register_object(_pm);
}

void GUI::remove_object(LEti::Object_2D* _object)
{
    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if((*it).object == _object)
        {
            if((*it).pm)
                m_collision_detector.unregister_object((*it).pm);
            m_objects.erase(it);
            return;
        }
    }
}



void GUI::M_update_cursor_position()
{
    m_cursor_position = m_camera.convert_window_coords({LR::Window_Controller::get_cursor_position().x, LR::Window_Controller::get_cursor_position().y, 0.0f});
}

void GUI::M_process_mouse_down()
{
    if(!LR::Window_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_1))
        return;

    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if(!(*it).on_pressed)
            continue;

        (*it).on_pressed(GLFW_MOUSE_BUTTON_1);  //  need to come up with some idea for passing different buttons here
    }
}

void GUI::M_process_mouse_up()
{
    if(!LR::Window_Controller::mouse_button_was_released(GLFW_MOUSE_BUTTON_1))
        return;

    for(auto it = m_objects.begin(); it.is_ok() && !it.end_reached(); ++it)
    {
        if(!(*it).on_released)
            continue;

        (*it).on_released(GLFW_MOUSE_BUTTON_1);
    }
}



void GUI::update_prev_state()
{
    for(auto it = m_objects.begin(); !it.end_reached(); ++it)
        (*it).object->update_previous_state();
}

void GUI::update(float _dt)
{
    for(auto it = m_objects.begin(); !it.end_reached(); ++it)
        (*it).object->update(_dt);

    m_collision_detector.update();
}
