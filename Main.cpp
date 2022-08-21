#include "Event_Controller.h"
#include "Shader.h"
#include "Camera.h"
#include "Resource_Loader.h"

#include <include/Object_System/Object_2D.h>
#include <include/Object_System/Text_Field.h>

#include "Physics/Physical_Model_2D.h"

#include "Physics/Space_Splitter_2D.h"
#include "Physics/Space_Hasher_2D.h"
#include "Physics/Default_Narrow_CD.h"
#include "Physics/Default_Narrowest_CD.h"

#include <include/Object_System/Object_2D.h>

#include "Timer.h"

//#include "Debug_Drawable_Frame.h"


#include <chrono>
#include <thread>


#define DT LEti::Event_Controller::get_dt()

/*
class Grid : public LEti::Object_2D
{
private:
	const float* m_default_c = nullptr;
	unsigned int m_default_c_count = 0;
	const float* m_default_tc = nullptr;
	unsigned int m_default_tc_count = 0;

	unsigned int m_grid_size = 0;

	void reconfigure_grid()
	{
		float min_x = m_default_c[0], max_x = m_default_c[0];
		float min_y = m_default_c[1], max_y = m_default_c[1];
		for(unsigned int i=0; i<m_default_c_count; i += 3)
		{
			if(min_x > m_default_c[i]) min_x = m_default_c[i];
			if(max_x < m_default_c[i]) max_x = m_default_c[i];
		}
		for(unsigned int i=1; i<m_default_c_count; i += 3)
		{
			if(min_y > m_default_c[i]) min_y = m_default_c[i];
			if(max_y < m_default_c[i]) max_y = m_default_c[i];
		}
		float width = max_x - min_x;
		float height = max_y - min_y;

		unsigned int c_count = m_grid_size * m_grid_size * m_default_c_count;
		float* coords_array = new float[c_count];
		for(unsigned int x=0; x<m_grid_size; ++x)
		{
			for(unsigned int y=0; y<m_grid_size; ++y)
			{
				for(unsigned int i=0; i<m_default_c_count; i += 3)
					coords_array[ (x * m_grid_size * m_default_c_count) + (y * m_default_c_count) + i ] = m_default_c[i] + (width * x);
				for(unsigned int i=1; i<m_default_c_count; i += 3)
					coords_array[ (x * m_grid_size * m_default_c_count) + (y * m_default_c_count) + i ] = m_default_c[i] + (height * y);
				for(unsigned int i=2; i<m_default_c_count; i += 3)
					coords_array[ (x * m_grid_size * m_default_c_count) + (y * m_default_c_count) + i ] = m_default_c[i];
			}
		}

		draw_module()->get_vertices().resize(c_count);
		draw_module()->get_vertices().copy_array(coords_array, c_count);
		draw_module()->get_vertices().setup_buffer(0, 3);

		delete[] coords_array;

//		std::cout << "\n";

		c_count = m_grid_size * m_grid_size * m_default_tc_count;
		coords_array = new float[c_count];
		for(unsigned int x=0; x<m_grid_size; ++x)
		{
			for(unsigned int y=0; y<m_grid_size; ++y)
			{
				for(unsigned int i=0; i<m_default_tc_count; ++i)
				{
//					std::cout << (x * m_grid_size * m_default_tc_count) + (y * m_default_tc_count) + i << "\n";
					coords_array[ (x * m_grid_size * m_default_tc_count) + (y * m_default_tc_count) + i ] = m_default_tc[i];
				}
			}
		}

		draw_module()->get_texture().resize(c_count);
		draw_module()->get_texture().copy_array(coords_array, c_count);
		draw_module()->get_texture().setup_buffer(1, 2);

		delete[] coords_array;
	}

public:
	Grid() { }
	~Grid() { }

	void init(const char *_object_name) override
	{
		remove_draw_module();
		remove_physics_module();

		auto translation = LEti::Resource_Loader::get_data<float>(_object_name, "position");
		ASSERT(translation.second != 3);
		set_pos({translation.first[0], translation.first[1], translation.first[2]});

		auto scale = LEti::Resource_Loader::get_data<float>(_object_name, "scale");
		ASSERT(scale.second != 3);
		set_scale({scale.first[0], scale.first[1], scale.first[2]});

		auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
		ASSERT(raxis.second != 3);
		set_rotation_axis({raxis.first[0], raxis.first[1], raxis.first[2]});

		auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
		ASSERT(rangle.second != 1);
		set_rotation_angle(*rangle.first);

		std::pair<const float*, unsigned int> tcoords;
		LEti::Resource_Loader::assign<float>(tcoords, _object_name, "texture_coords");

		if(tcoords.first)
		{
			create_draw_module();
			m_draw_module->init_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "texture_name").first->c_str(), tcoords.first, tcoords.second);
			auto coords = LEti::Resource_Loader::get_data<float>(_object_name, "coords");
			m_draw_module->init_vertices(coords.first, coords.second);
			m_default_c = coords.first;
			m_default_c_count = coords.second;
			m_default_tc = tcoords.first;
			m_default_tc_count = tcoords.second;
		}

		std::pair<const float*, unsigned int> physical_model_data;
		LEti::Resource_Loader::assign(physical_model_data, _object_name, "physical_model_data");

		if((physical_model_data.first))
		{
			create_physics_module();
			m_physics_module->init(physical_model_data.first, physical_model_data.second);
		}
	}

	void set_grid_size(unsigned int _size)
	{
		m_grid_size = _size;
		reconfigure_grid();
	}

};
*/

int main()
{
	LEti::Window_Controller::create_window(1200, 800, "Collision Test");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);

	LEti::Shader::init_shader("Resources/Shaders/vertex_shader.shader", "Resources/Shaders/fragment_shader.shader");
	ASSERT(!LEti::Shader::is_valid());
	LEti::Shader::set_texture_uniform_name("input_texture");
	LEti::Shader::set_transform_matrix_uniform_name("transform_matrix");
	LEti::Shader::set_projection_matrix_uniform_name("projection_matrix");

	LEti::Camera::setup_orthographic_matrix();
	LEti::Event_Controller::set_max_dt(60.0f / 1000.0f);

	LEti::Space_Splitter_2D::set_broad_phase<LEti::Space_Hasher_2D>();
	LEti::Space_Splitter_2D::set_narrow_phase<LEti::Default_Narrow_CD>();
	LEti::Space_Splitter_2D::set_narrowest_phase<LEti::Default_Narrowest_CD>();
	LEti::Space_Splitter_2D::get_broad_phase()->set_precision(10);
	LEti::Space_Splitter_2D::get_narrow_phase()->set_precision(10);

	LEti::Resource_Loader::init();
	LEti::Resource_Loader::load_object("textures", "Resources/Textures/textures.mdl");

	LEti::Resource_Loader::load_object("grid_block", "Resources/Models/grid_block.mdl");

	LEti::Object_2D test_object;
	test_object.init("grid_block");

//	Grid grid;
//	grid.init("grid_block");

//	unsigned int grid_size = 3;
//	grid.set_grid_size(grid_size);
//	grid.set_pos({50.0f, 50.0f, 0.0f});
//	grid_block.set_overall_scale(30.0f);


	LEti::Timer fps_timer;


	auto reset_func = [&]()
	{

	};
	reset_func();


	LEti::Resource_Loader::load_object("text_field", "Resources/Models/text_field.mdl");

	LEti::Text_Field fps_info_block;
	fps_info_block.init("text_field");
	fps_info_block.set_pos({10, 770, 0});

	unsigned int fps_counter = 0;

	while (!LEti::Window_Controller::window_should_close())
	{
		LEti::Event_Controller::update();
		LEti::Window_Controller::update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LEti::Camera::update(false, true);


//		if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_UP))
//		{
//			++grid_size;
//			grid.set_grid_size(grid_size);
//		}
//		if(LEti::Event_Controller::key_was_pressed(GLFW_KEY_DOWN))
//		{
//			if(grid_size > 1)
//			{
//				--grid_size;
//				grid.set_grid_size(grid_size);
//			}
//		}

		if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
		{
			test_object.move({0.0f, 200.0f * DT, 0.0f});
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
		{
			test_object.move({0.0f, -200.0f * DT, 0.0f});
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
		{
			test_object.move({-200.0f * DT, 0.0f, 0.0f});
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
		{
			test_object.move({200.0f * DT, 0.0f, 0.0f});
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_Q))
		{
			test_object.rotate(LEti::Math::PI * DT);
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_E))
		{
			test_object.rotate(-LEti::Math::PI * DT);
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_SPACE))
		{
			test_object.set_scale(test_object.get_scale() * (1.0f + DT));
		}
		if(LEti::Event_Controller::is_key_down(GLFW_KEY_LEFT_SHIFT))
		{
			test_object.set_scale(test_object.get_scale() / (1.0f + DT));
		}




		LEti::Space_Splitter_2D::update();




//		grid.draw();


		test_object.draw();


		++fps_counter;
		fps_timer.update();
		if(!fps_timer.is_active())
		{
			fps_timer.start(1.0f);
			fps_info_block.set_text((std::to_string(fps_counter)).c_str());
			fps_counter = 0;
		}
		fps_info_block.draw();

		LEti::Window_Controller::swap_buffers();
	}

	return 0;
}














































