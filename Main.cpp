#include <chrono>
#include <thread>

#include <MDL_Reader.h>

#include <Stuff/Timer.h>
#include <Event_Controller.h>
#include <Shader.h>
#include <Camera_2D.h>
#include <Picture_Manager.h>
#include <Physics/Collision_Detector_2D.h>
#include <Physics/Collision_Resolver.h>
#include <Physics/Collision_Resolution__Rigid_Body_2D.h>
#include <Physics/Space_Hasher_2D.h>
#include <Physics/Dynamic_Narrow_CD.h>
#include <Physics/Default_Narrowest_CD.h>
#include <Physics/SAT_Narrowest_CD.h>
#include <Object_System/Text_Field.h>
#include <Object_System/Object_2D.h>
#include <Object_System/Rigid_Body_2D_Stub.h>
#include <Object_System/Modules/Draw_Module__Animation.h>
#include <Renderer.h>

#include <Game_World/Background.h>
#include <Game_World/Collision_Resolution__Entity.h>
#include <Game_World/Entity_Manager.h>
#include <Game_World/Enemy_Generator.h>
#include <Game_World/Player_Controller.h>
#include <Game_World/Player.h>
#include <Game_World/Enemy.h>
#include <Game_World/Game_World.h>
#include <GUI.h>
#include <Effects_Controller.h>
#include <Edit_Mode/Edit_Mode.h>
#include <Edit_Mode/Grid.h>


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


    glm::vec3 cursor_position(0.0f, 0.0f, 0.0f);


    LV::MDL_Reader reader;

    LEti::Window_Controller::create_window(1200, 800, "Generic Space Shooter Game");

    //    glEnable(GL_DEPTH_TEST);
    //    glClearColor(0.0,0.0,0.0,0.0);

    glEnable(GL_SCISSOR_TEST);
//    glScissor(20, 20, 200, 200);

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
    reader.parse_file("Resources/Models/explosion");
    reader.parse_file("Resources/Models/projectile");
    reader.parse_file("Resources/Models/grid_cell");
    reader.parse_file("Resources/Models/blocks");

    LEti::Text_Field_Stub text_field_stub;
    text_field_stub.assign_values(reader.get_stub("text_field"));

    LEti::Text_Field* player_hp_tf = (LEti::Text_Field*)text_field_stub.construct();
    player_hp_tf->set_pos({20.0f, 20.0f, 0.0f});
    player_hp_tf->set_text(" ");

    LEti::Text_Field* player_respawn_timer_tf = (LEti::Text_Field*)text_field_stub.construct();
    player_respawn_timer_tf->set_pos({600.0f, 400.0f, 0.0f});
    player_respawn_timer_tf->set_horizontal_alignment(LEti::Text_Field::Horizontal_Alignment::center);
    player_respawn_timer_tf->set_vertical_alignment(LEti::Text_Field::Vertical_Alignment::center);

    LEti::Text_Field* player_eliminations_tf = (LEti::Text_Field*)text_field_stub.construct();
    player_eliminations_tf->set_pos({1180, 20.0f, 0.0f});
    player_eliminations_tf->set_horizontal_alignment(LEti::Text_Field::Horizontal_Alignment::right);

    gui.add_object(player_hp_tf);
    gui.add_object(player_respawn_timer_tf);
    gui.add_object(player_eliminations_tf);


    GSSG::Player_Stub player_stub;
//    player_stub.draw_module = new LEti::Draw_Module__Animation__Stub;
    player_stub.draw_module = new LEti::Default_Draw_Module_2D_Stub;
    player_stub.physics_module = new LEti::Physics_Module__Rigid_Body_2D__Stub;
    player_stub.assign_values(reader.get_stub("arrow_quad"));
    player_stub.health = 5;
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)player_stub.physics_module)->masses = new float[2];
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)player_stub.physics_module)->masses[0] = 5.0f;
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)player_stub.physics_module)->masses[1] = 5.0f;

    GSSG::Enemy_Stub enemy_entity_stub;
    enemy_entity_stub.draw_module = new LEti::Default_Draw_Module_2D_Stub;
    enemy_entity_stub.physics_module = new LEti::Physics_Module__Rigid_Body_2D__Stub;
    enemy_entity_stub.assign_values(reader.get_stub("triangle"));
    enemy_entity_stub.health = 1;
    ((LEti::Default_Draw_Module_2D_Stub*)enemy_entity_stub.draw_module)->texture_name = "triangle_texture";

    GSSG::Projectile_Stub projectile_stub;
    projectile_stub.draw_module = new LEti::Draw_Module__Animation__Stub;
    projectile_stub.physics_module = new LEti::Physics_Module__Rigid_Body_2D__Stub;
    projectile_stub.assign_values(reader.get_stub("projectile"));
    projectile_stub.scale = { 8.0f, 8.0f, 1.0f };
    projectile_stub.health = 1;
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)projectile_stub.physics_module)->masses = new float[2];
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)projectile_stub.physics_module)->masses[0] = 2.5f;
    ((LEti::Physics_Module__Rigid_Body_2D__Stub*)projectile_stub.physics_module)->masses[1] = 2.5f;

    LEti::Object_2D_Stub explosion_stub;
    explosion_stub.draw_module = new LEti::Draw_Module__Animation__Stub;
    explosion_stub.assign_values(reader.get_stub("explosion"));


    GSSG::Effects_Controller effects_controller;
    effects_controller.inject_renderer(&renderer);

    GSSG::Enemy_Generator enemy_generator;
    enemy_generator.set_spawn_frequency(30000.0f);
    enemy_generator.inject_entity_manager(&entity_manager);
    enemy_generator.inject_camera(&camera);
    enemy_generator.set_enemy_stub(&enemy_entity_stub);
    enemy_generator.set_enemy_projectile_stub(&projectile_stub);

    GSSG::Player_Controller player_controller;
    player_controller.set_player_stub(&player_stub);
    player_controller.set_projectile_stub(&projectile_stub);
    player_controller.inject_camera(&camera);
    player_controller.inject_entity_manager(&entity_manager);
    player_controller.inject_player_hp_caption(player_hp_tf);
    player_controller.inject_player_respawn_caption(player_respawn_timer_tf);
    player_controller.inject_player_eliminations_amount_caption(player_eliminations_tf);

    player_stub.effects_controller = &effects_controller;
    player_stub.on_death_effect = &explosion_stub;
    enemy_entity_stub.effects_controller = &effects_controller;
    enemy_entity_stub.on_death_effect = &explosion_stub;
    projectile_stub.effects_controller = &effects_controller;
    projectile_stub.on_death_effect = &explosion_stub;

    player_controller.update();
    entity_manager.update_entities_prev_state();
    entity_manager.update_entities(0.0f);

    //  ~game setup


    //  game logic setup

    GSSG::Game_World* game_world = new GSSG::Game_World;
    game_world->set_entity_manager(&entity_manager);
    game_world->set_enemy_generator(&enemy_generator);
    game_world->set_player_controller(&player_controller);
    game_world->set_effects_controller(&effects_controller);
    game_world->set_gui(&gui);
    game_world->set_background(&background);
    game_world->set_collision_detector(&collision_detector);
    game_world->set_collision_resolver(&collision_resolver);
    game_world->set_renderer(&renderer);

    GSSG::Game_Logic* game_logic = game_world;



    LEti::Object_2D_Stub em_cell_stub;
    em_cell_stub.draw_module = new LEti::Default_Draw_Module_2D_Stub;
    em_cell_stub.physics_module = new LEti::Dynamic_Physics_Module_2D_Stub;
    em_cell_stub.assign_values(reader.get_stub("grid_cell"));



    GSSG::Block_Controller block_controller;
    block_controller.init(reader.get_stub("blocks"));

    LEti::Collision_Detector_2D grid_collision_detector;
    grid_collision_detector.set_broad_phase(new LEti::Space_Hasher_2D, 10);
    grid_collision_detector.set_narrow_phase(new LEti::Dynamic_Narrow_CD, 10);
    grid_collision_detector.set_narrowest_phase(new LEti::SAT_Narrowest_CD);
    grid_collision_detector.register_point(&cursor_position);

    GSSG::Grid grid;
    grid.set_renderer(&renderer);
    grid.set_cell_stub(&em_cell_stub);
    grid.set_collision_detector(&grid_collision_detector);
    grid.set_block_controller(&block_controller);
    grid.construct(7, 7);


    GSSG::Edit_Mode* edit_mode = new GSSG::Edit_Mode;
    edit_mode->set_camera(&camera);
    edit_mode->set_player_controller(&player_controller);
    edit_mode->set_player_stub(&player_stub);
    edit_mode->set_grid(&grid);
    edit_mode->set_block_controller(&block_controller);

    //  ~game logic setup


    LST::Timer fps_timer;

    collision_detector.register_point(&cursor_position);

    unsigned int fps_counter = 0;

    //    enemy_generator.update();

    while (!LEti::Window_Controller::window_should_close())
    {
        LEti::Window_Controller::update();
        LEti::Event_Controller::update();

        cursor_position = camera.convert_window_coords(glm::vec3(LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f));

        glClear(GL_COLOR_BUFFER_BIT);

        game_logic->update();

        if(LEti::Event_Controller::key_was_released(GLFW_KEY_TAB))
        {
            game_logic->on_deactivate();

            if(game_logic == game_world)
                game_logic = edit_mode;
            else
                game_logic = game_world;

            game_logic->on_activate();
        }

        if(LEti::Event_Controller::key_was_released(GLFW_KEY_B))
            enemy_generator.spawn_enemy();

        ++fps_counter;
        fps_timer.update(LEti::Event_Controller::get_dt());

        if(!fps_timer.is_active())
        {
            fps_timer.start(1.0f);
            fps_counter = 0;
        }

        LEti::Window_Controller::swap_buffers();
    }

    delete player_hp_tf;
    delete player_respawn_timer_tf;
    delete player_eliminations_tf;

    delete game_world;
    delete edit_mode;

    return 0;
}














































