#include "Event_Controller.h"
#include "Shader.h"
#include "Camera_2D.h"
#include "Picture_Manager.h"

#include "Object_System/Text_Field.h"

#include <Object_System/Object_2D.h>

#include "Physics/Physical_Model_3D.h"
#include "Physics/Physical_Model_2D.h"

#include "Physics/Collision_Detector_2D.h"
#include "Physics/Collision_Resolver.h"
#include "Physics/Collision_Resolution__Rigid_Body_2D.h"

#include "Physics/Space_Hasher_2D.h"
#include "Physics/Dynamic_Narrow_CD.h"
#include "Physics/Default_Narrowest_CD.h"
#include "Physics/SAT_Narrowest_CD.h"

#include "Timer.h"

#include "Object_System/Debug_Drawable_Frame.h"

#include <sstream>
#include <iomanip>

#include <chrono>
#include <thread>


#include <Object_System/Rigid_Body_2D.h>

#include <Renderer.h>

#include "MDL_Reader.h"

int main()
{
	LV::Type_Manager::register_type("int", {
										[](const std::string& _val)
										{
											unsigned int i=0;
											if(_val[0] == '+' || _val[0] == '-')
											++i;
											for(; i<_val.size(); ++i)
											if(_val[i] < '0' || _val[i] > '9')
											return false;
											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((int*)_variable_vptr) = std::stoi(_values_as_string[0]); }
									});
	LV::Type_Manager::register_type("unsigned int", {
										[](const std::string& _val)
										{
											for(unsigned int i=0; i<_val.size(); ++i)
											if(_val[i] < '0' || _val[i] > '9')
											return false;
											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((int*)_variable_vptr) = std::stoi(_values_as_string[0]); }
									});
	LV::Type_Manager::register_type("bool", {
										[](const std::string& _val)
										{
											if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
												return true;
											return false;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
										{
											bool& var = *((bool*&)_variable_vptr);

											if(_values_as_string[0] == "true" || _values_as_string[0] == "+" || _values_as_string[0] == "1")
												var = true;
											else if(_values_as_string[0] == "false" || _values_as_string[0] == "-" || _values_as_string[0] == "0")
												var = false;

//											*((bool*&)_variable_vptr) = _values_as_string[0] == "true" ? true : false;
										}
									});
	LV::Type_Manager::register_type("bool*", {
										[](const std::string& _val)
										{
											if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
												return true;
											return false;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
										{
											bool** var_ptr_ptr = (bool**)_variable_vptr;

											if(*var_ptr_ptr == nullptr)
											*var_ptr_ptr = new bool[_values_as_string.size()];

											bool* var_ptr = *var_ptr_ptr;

											for(unsigned int i=0; i<_values_as_string.size(); ++i)
											{
												if(_values_as_string[i] == "true" || _values_as_string[i] == "+" || _values_as_string[i] == "1")
													var_ptr[i] = true;
												else if(_values_as_string[i] == "false" || _values_as_string[i] == "-" || _values_as_string[i] == "0")
													var_ptr[i] = false;
											}
										}
									});
	LV::Type_Manager::register_type("std::string", {
										[](const std::string& _val) { return true; },
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) {
											*((std::string*)_variable_vptr) = _values_as_string[0];
										}
									});
	LV::Type_Manager::register_type("float*", {
										[](const std::string& _val)
										{
											if(_val == ".")
											return false;

											unsigned int dots_count = 0;
											unsigned int i=0;
											if(_val[0] == '+' || _val[0] == '-')
											++i;
											for(; i<_val.size(); ++i)
											{
												if(_val[i] == '.')
												{
													++dots_count;
													continue;
												}
												if(_val[i] < '0' || _val[i] > '9')
												return false;
											}

											if(dots_count > 1)
											return false;

											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
										{
											float** var_ptr_ptr = (float**)_variable_vptr;

											if(*var_ptr_ptr == nullptr)
											*var_ptr_ptr = new float[_values_as_string.size()];

											float* var_ptr = *var_ptr_ptr;

											for(unsigned int i=0; i<_values_as_string.size(); ++i)
											var_ptr[i] = std::stof(_values_as_string[i]);
										}
									});
	LV::Type_Manager::register_type("std::string*", {
										[](const std::string& /*_val*/)
										{
											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
										{
											std::string** var_ptr_ptr = (std::string**)_variable_vptr;

											if(*var_ptr_ptr == nullptr)
											*var_ptr_ptr = new std::string[_values_as_string.size()];

											std::string* var_ptr = *var_ptr_ptr;

											for(unsigned int i=0; i<_values_as_string.size(); ++i)
											var_ptr[i] = _values_as_string[i];
										}
									});
	LV::Type_Manager::register_type("glm::vec3", {
										[](const std::string& _val)
										{
											if(_val == ".")
											return false;

											unsigned int dots_count = 0;
											unsigned int i=0;
											if(_val[0] == '+' || _val[0] == '-')
											++i;
											for(; i<_val.size(); ++i)
											{
												if(_val[i] == '.')
												{
													++dots_count;
													continue;
												}
												if(_val[i] < '0' || _val[i] > '9')
												return false;
											}

											if(dots_count > 1)
											return false;

											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
										{
											L_ASSERT(_values_as_string.size() == 3);

											glm::vec3& vec = *((glm::vec3*)_variable_vptr);
											for(unsigned int i=0; i<3; ++i)
											vec[i] = std::stof(_values_as_string[i]);
										}
									});
	LV::Type_Manager::register_type("float", {
										[](const std::string& _val)
										{
											if(_val == ".")
											return false;

											unsigned int dots_count = 0;
											unsigned int i=0;
											if(_val[0] == '+' || _val[0] == '-')
											++i;
											for(; i<_val.size(); ++i)
											{
												if(_val[i] == '.')
												{
													++dots_count;
													continue;
												}
												if(_val[i] < '0' || _val[i] > '9')
												return false;
											}

											if(dots_count > 1)
											return false;

											return true;
										},
										[](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((float*)_variable_vptr) = std::stof(_values_as_string[0]); }
                                    });

	LV::MDL_Reader reader;
//    reader.parse_file("Resources/Models/quad_new");

//	LEti::Object_2D_Stub quad;
//	quad.assign_values(reader.get_stub("quad"));

    LEti::Window_Controller::create_window(1200, 800, "Generic Space Shooter Game");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CW);

    LEti::Camera_2D camera;

//    camera.set_position({600, 40, 0});
//    camera.set_view_scale(2.0f);

    LEti::Shader shader;

    LEti::Renderer renderer;
    renderer.set_camera(&camera);
    renderer.set_shader(&shader);

    shader.init("Resources/Shaders/vertex_shader.shader", "Resources/Shaders/fragment_shader.shader");
    shader.set_texture_uniform("input_texture");
    shader.set_transform_matrix_uniform("transform_matrix");
    shader.set_projection_matrix_uniform("projection_matrix");

	LEti::Collision_Detector_2D collision_detector;

	collision_detector.set_broad_phase(new LEti::Space_Hasher_2D, 10);
	collision_detector.set_narrow_phase(new LEti::Dynamic_Narrow_CD, 10);
	collision_detector.set_narrowest_phase(new LEti::SAT_Narrowest_CD);

	LEti::Collision_Resolver Collision_Resolver;
	Collision_Resolver.add_resolution(new LEti::Collision_Resolution__Rigid_Body_2D);

	reader.parse_file("Resources/Textures/textures");
	LEti::Picture_Manager::Picture_Autoload_Stub texture_autoload;
	texture_autoload.assign_values(reader.get_stub("textures"));


	LEti::Event_Controller::set_max_dt(60.0f / 1000.0f);


//	L_CREATE_LOG_LEVEL("MOUSE_POS_LL");


	///////////////// 2d collision test

//	LEti::Rigid_Body_2D flat_co;
//	flat_co.init(quad);
//	flat_co.set_mass(4.0f);
//	flat_co.set_pos({800, 400, 0});
//	flat_co.draw_module()->set_texture(LEti::Picture_Manager::get_picture("white_texture"));

    LEti::Timer fps_timer;

//    co.second->update();

	glm::vec3 cursor_position(0.0f, 0.0f, 0.0f);

	collision_detector.register_point(&cursor_position);

//    collision_detector.register_object(co.second);

	unsigned int fps_counter = 0;

	while (!LEti::Window_Controller::window_should_close())
	{
		LEti::Event_Controller::update();
		LEti::Window_Controller::update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        co.second->update_previous_state();

		if (LEti::Event_Controller::key_was_pressed(GLFW_KEY_K))
		{
        }

		if(LEti::Event_Controller::mouse_wheel_rotation() != 0)
		{
			float additional_scale_per_rotation = 0.2f;

			additional_scale_per_rotation *= -(float)(LEti::Event_Controller::mouse_wheel_rotation());

            camera.set_view_scale(camera.view_scale() + additional_scale_per_rotation);
        }

//        co.second->update();

//		LEti::Camera_2D::set_position(flat_co.get_pos());

		if(LEti::Event_Controller::mouse_button_was_pressed(GLFW_MOUSE_BUTTON_1))
        {
        }

		collision_detector.update();

		Collision_Resolver.resolve_all(collision_detector.get_collisions__models());

//        renderer.draw(*co.second->draw_module());

		++fps_counter;
		fps_timer.update();

		if(!fps_timer.is_active())
		{
            fps_timer.start(1.0f);
			fps_counter = 0;
        }

		LEti::Window_Controller::swap_buffers();
	}

	return 0;
}














































