#ifndef GUI_H
#define GUI_H

#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>

#include <Renderer.h>
#include <Object_System/Object_2D.h>
#include <Physics/Collision_Detector_2D.h>
#include <Physics/Space_Hasher_2D.h>
#include <Physics/Dynamic_Narrow_CD.h>
#include <Physics/SAT_Narrowest_CD.h>


namespace GSSG
{

    class GUI
    {
    private:
        struct GUI_Element_Data
        {
            LEti::Object_2D* object = nullptr;
            LST::Function_Wrapper<void, unsigned int>* on_pressed = nullptr;
            LST::Function_Wrapper<void, unsigned int>* on_released = nullptr;

            GUI_Element_Data(LEti::Object_2D* _object, LST::Function_Wrapper<void, unsigned int>* _on_pressed, LST::Function_Wrapper<void, unsigned int>* _on_released)
                : object(_object), on_pressed(_on_pressed), on_released(_on_released) { }
            ~GUI_Element_Data() { delete on_pressed; delete on_released; }
        };

    private:
        LEti::Renderer m_renderer;
        LEti::Camera_2D m_camera;

        LEti::Collision_Detector_2D m_collision_detector;

        LDS::List<GUI_Element_Data> m_objects;
        glm::vec3 m_cursor_position{0.0f, 0.0f, 0.0f};

    public:
        GUI();
        ~GUI();

    public:
        inline void inject_shader(const LEti::Shader* _shader) { m_renderer.set_shader(_shader); }

    public:
        void add_object(LEti::Object_2D* _object, LST::Function_Wrapper<void, unsigned int>* _on_pressed = nullptr, LST::Function_Wrapper<void, unsigned int>* _on_released = nullptr);
        void remove_object(LEti::Object_2D* _object);

    private:
        void M_update_cursor_position();
        void M_process_mouse_down();
        void M_process_mouse_up();

    public:
        void update_prev_state();
        void update(float _ratio = 1.0f);
        void draw();

    };

}


#endif // GUI_H
