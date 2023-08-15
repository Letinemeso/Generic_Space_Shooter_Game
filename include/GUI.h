#ifndef GUI_H
#define GUI_H

#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>

#include <Renderer/Renderer.h>
#include <Object_System/Object_2D.h>
#include <Collision_Detection/Collision_Detector_2D.h>
#include <Collision_Detection/Space_Hasher_2D.h>
#include <Collision_Detection/Dynamic_Narrow_CD.h>
#include <Collision_Detection/SAT_Narrowest_CD.h>

#include <Camera/Camera_2D.h>


namespace GSSG
{

    class GUI
    {
    private:
        struct GUI_Element_Data
        {
            LEti::Object_2D* object = nullptr;
            LPhys::Physics_Module_2D* pm = nullptr;
            LST::Function<void(unsigned int)> on_pressed;
            LST::Function<void(unsigned int)> on_released;

            GUI_Element_Data(LEti::Object_2D* _object, LPhys::Physics_Module_2D* _pm, const LST::Function<void(unsigned int)>& _on_pressed, const LST::Function<void(unsigned int)>& _on_released)
                : object(_object), pm(_pm), on_pressed(_on_pressed), on_released(_on_released) { }
            ~GUI_Element_Data() { }
        };

    private:
        LR::Renderer m_renderer;
        LR::Camera_2D m_camera;

        LPhys::Collision_Detector_2D m_collision_detector;

        LDS::List<GUI_Element_Data> m_objects;
        glm::vec3 m_cursor_position{0.0f, 0.0f, 0.0f};

    public:
        GUI();
        ~GUI();

    public:
        inline void inject_shader(const LR::Shader* _shader) { m_renderer.set_shader(_shader); }

    public:
        void add_object(LEti::Object_2D* _object, LPhys::Physics_Module_2D* _pm = nullptr, const LST::Function<void(unsigned int)>& _on_pressed = {}, const LST::Function<void(unsigned int)>& _on_released = {});
        void remove_object(LEti::Object_2D* _object);

    private:
        void M_update_cursor_position();
        void M_process_mouse_down();
        void M_process_mouse_up();

    public:
        void update_prev_state();
        void update(float _dt);

    };

}


#endif // GUI_H
