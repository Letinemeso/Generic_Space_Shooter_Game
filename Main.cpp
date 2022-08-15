#include "Event_Controller.h"
#include "Shader.h"
#include "Camera.h"
#include "Resource_Loader.h"

#include "Object.h"
#include "Text_Field.h"

#include "Physics/Physical_Model_2D.h"

#include "Physics/Space_Splitter_2D.h"
#include "Physics/Space_Hasher_2D.h"
#include "Physics/Default_Narrow_CD.h"
#include "Physics/Default_Narrowest_CD.h"

#include "Timer.h"

#include "Debug_Drawable_Frame.h"


#include <chrono>
#include <thread>


#define DT LEti::Event_Controller::get_dt()

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
		float* coords_array = new float[m_grid_size * m_grid_size];

	}

public:
	Grid() { }
	~Grid() { }

	void init(const char *_object_name) override
	{
		auto tcoords = LEti::Resource_Loader::get_data<float>(_object_name, "texture_coords");
		init_texture(LEti::Resource_Loader::get_data<std::string>(_object_name, "texture_name").first->c_str(), tcoords.first, tcoords.second);

		m_default_tc_count = tcoords.second;
		m_default_tc = tcoords.first;

		auto coords = LEti::Resource_Loader::get_data<float>(_object_name, "coords");
		init_vertices(coords.first, coords.second);

		m_default_c_count = coords.second;
		m_default_c = coords.first;

		auto translation = LEti::Resource_Loader::get_data<float>(_object_name, "position");
		ASSERT(translation.second != 3);
		set_pos(translation.first[0], translation.first[1], translation.first[2]);

		auto scale = LEti::Resource_Loader::get_data<float>(_object_name, "scale");
		ASSERT(scale.second != 3);
		set_scale(scale.first[0], scale.first[1], scale.first[2]);

		auto raxis = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_axis");
		ASSERT(raxis.second != 3);
		set_rotation_axis(raxis.first[0], raxis.first[1], raxis.first[2]);

		auto rangle = LEti::Resource_Loader::get_data<float>(_object_name, "rotation_angle");
		ASSERT(rangle.second != 1);
		set_rotation_angle(*rangle.first);

	}

	void set_grid_size(unsigned int _size)
	{
		m_grid_size = _size;
	}

};

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


	LEti::Object_2D grid_block;
	grid_block.init("grid_block");
	grid_block.set_pos(600.0f, 400.0f, 0.0f);


	LEti::Timer fps_timer;


	auto reset_func = [&]()
	{

	};
	reset_func();


	LEti::Resource_Loader::load_object("text_field", "Resources/Models/text_field.mdl");

	LEti::Text_Field fps_info_block;
	fps_info_block.init("text_field");
	fps_info_block.set_pos(10, 770, 0);

	unsigned int fps_counter = 0;

	while (!LEti::Window_Controller::window_should_close())
	{
		LEti::Event_Controller::update();
		LEti::Window_Controller::update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LEti::Camera::update(false, true);






		LEti::Space_Splitter_2D::update();




		grid_block.draw();





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














































