#include <chrono>
#include <thread>

#include <MDL_Reader.h>

#include <Timer.h>
#include <Event_Controller.h>
#include <Shader.h>
#include <Camera_2D.h>
#include <Picture_Manager.h>
#include <Physics/Physical_Model_2D.h>
#include <Physics/Collision_Detector_2D.h>
#include <Physics/Collision_Resolver.h>
#include <Physics/Collision_Resolution__Rigid_Body_2D.h>
#include <Physics/Space_Hasher_2D.h>
#include <Physics/Dynamic_Narrow_CD.h>
#include <Physics/Default_Narrowest_CD.h>
#include <Physics/SAT_Narrowest_CD.h>
#include <Object_System/Text_Field.h>
#include <Object_System/Object_2D.h>
#include <Object_System/Rigid_Body_2D.h>
#include <Renderer.h>

#include <Background.h>
#include <Collision_Resolution__Entity.h>
#include <Entity_Manager.h>
#include <Enemy_Generator.h>
#include <Player_Controller.h>
#include <Player.h>
#include <Enemy.h>
#include <GUI.h>


int main()
{
    //  engine setup

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
    srand(time(nullptr));

    LV::MDL_Reader reader;

    LEti::Window_Controller::create_window(1200, 800, "Generic Space Shooter Game");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_CW);

    LEti::Shader shader;

    LEti::Camera_2D camera;
    camera.set_view_scale(1.0f);

    LEti::Renderer renderer;
    renderer.set_camera(&camera);
    renderer.set_shader(&shader);

    GSSG::GUI gui;
    gui.inject_shader(&shader);

    shader.init("Resources/Shaders/vertex_shader.shader", "Resources/Shaders/fragment_shader.shader");
    shader.set_texture_uniform("input_texture");
    shader.set_transform_matrix_uniform("transform_matrix");
    shader.set_projection_matrix_uniform("projection_matrix");

	LEti::Collision_Detector_2D collision_detector;

	collision_detector.set_broad_phase(new LEti::Space_Hasher_2D, 10);
	collision_detector.set_narrow_phase(new LEti::Dynamic_Narrow_CD, 10);
	collision_detector.set_narrowest_phase(new LEti::SAT_Narrowest_CD);

    LEti::Collision_Resolver collision_resolver;
    collision_resolver.add_resolution(new GSSG::Collision_Resolution__Entity);
    collision_resolver.add_resolution(new LEti::Collision_Resolution__Rigid_Body_2D);

	reader.parse_file("Resources/Textures/textures");
	LEti::Picture_Manager::Picture_Autoload_Stub texture_autoload;
	texture_autoload.assign_values(reader.get_stub("textures"));


	LEti::Event_Controller::set_max_dt(60.0f / 1000.0f);

    //  ~engine setup


    //  game setup

    GSSG::Background background;
    background.inject_camera(&camera);
    background.set_picture(LEti::Picture_Manager::get_picture("background_texture"));

    GSSG::Entity_Manager entity_manager;
    entity_manager.inject_collision_detector(&collision_detector);
    entity_manager.inject_renderer(&renderer);
    entity_manager.inject_camera(&camera);
    entity_manager.set_max_distance_from_view_pos(1000.0f);

    reader.parse_file("Resources/Models/arrow_quad");
    reader.parse_file("Resources/Models/triangle");
    reader.parse_file("Resources/Models/text_field_new");

    LEti::Text_Field_Stub text_field_stub;
    text_field_stub.assign_values(reader.get_stub("text_field"));

    LEti::Text_Field player_hp_tf;
    player_hp_tf.init(text_field_stub);
    player_hp_tf.set_pos({20.0f, 20.0f, 0.0f});
    player_hp_tf.set_text("abc");

    gui.add_object(&player_hp_tf);


    LEti::Object_2D_Stub arrow_quad_stub;
    arrow_quad_stub.assign_values(reader.get_stub("arrow_quad"));

    LEti::Object_2D_Stub enemy_entity_stub;
    enemy_entity_stub.assign_values(reader.get_stub("triangle"));
    enemy_entity_stub.texture_name = "triangle_texture";

    LEti::Object_2D_Stub projectile_stub;
    projectile_stub.assign_values(reader.get_stub("triangle"));
    projectile_stub.scale = { 3.0f, 3.0f, 1.0f };

    GSSG::Enemy_Generator enemy_generator;
    enemy_generator.set_spawn_frequency(5.0f);
    enemy_generator.inject_entity_manager(&entity_manager);
    enemy_generator.inject_camera(&camera);
    enemy_generator.set_enemy_stub(&enemy_entity_stub);
    enemy_generator.set_enemy_projectile_stub(&projectile_stub);

    GSSG::Player_Controller player_controller;
    player_controller.set_player_stub(&arrow_quad_stub);
    player_controller.set_projectile_stub(&projectile_stub);
    player_controller.inject_camera(&camera);
    player_controller.inject_entity_manager(&entity_manager);
    player_controller.inject_player_hp_caption(&player_hp_tf);
    player_controller.update();

    entity_manager.update_entities_prev_state();
    entity_manager.update_entities(0.0f);

    //  ~game setup


    LEti::Timer fps_timer;

	glm::vec3 cursor_position(0.0f, 0.0f, 0.0f);

    collision_detector.register_point(&cursor_position);

	unsigned int fps_counter = 0;

//    enemy_generator.update();

	while (!LEti::Window_Controller::window_should_close())
	{
		LEti::Event_Controller::update();
		LEti::Window_Controller::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        entity_manager.update_entities_prev_state();
        entity_manager.apply_entities_input();
        entity_manager.update_entities();

        gui.update_prev_state();

        collision_detector.update();
        collision_resolver.resolve_all(collision_detector.get_collisions__models());

        entity_manager.remove_dead_entities();
        enemy_generator.update();
        player_controller.update();

        gui.update();

        background.update();

        gui.draw();

        entity_manager.draw_entities();

        renderer.draw(*background.draw_module());

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














































