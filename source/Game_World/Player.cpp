#include <Game_World/Player.h>

#include <Game_World/Player_Controller.h>
#include <Edit_Mode/Block_Controller.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Player_Stub, LEti::Builder_Stub)
FIELDS_END



void Player_Stub::M_init_constructed_product(LV::Variable_Base *_product) const
{
    Player* product = (Player*)_product;

    LEti::Default_Draw_Module_2D* dm = new LEti::Default_Draw_Module_2D;
    dm->assign_values({});
    Physics_Module__Space_Ship* pm = new Physics_Module__Space_Ship;
    pm->assign_values({});

    product->set_draw_module(dm);
    product->set_physics_module(pm);

    product->set_structure(structure);

    product->set_health(health);

    product->inject_effects_controller(effects_controller);
    product->set_on_death_effect(on_death_effect);

    product->draw_module()->set_texture(picture);

    product->reconstruct();

    product->set_scale({100.0f, 100.0f, 1.0f});
    product->set_pos({0.0f, 0.0f, 0.0f});
    product->set_rotation_axis({0.0f, 0.0f, 1.0f});
    product->set_on_update_func([product](float _ratio)
    {
        LEti::Physics_Module__Rigid_Body_2D* pm = (LEti::Physics_Module__Rigid_Body_2D*)product->physics_module();

        product->move(pm->velocity() * LEti::Event_Controller::get_dt() * _ratio);
        product->rotate(pm->angular_velocity() * LEti::Event_Controller::get_dt() * _ratio);
    });
}



INIT_FIELDS(GSSG::Player, GSSG::Space_Ship)
FIELDS_END



Player::Player()
{

}

Player::~Player()
{

}



void Player::inject_player_hp_caption(LEti::Text_Field *_player_hp_tf)
{
    m_player_hp_tf = _player_hp_tf;
    m_player_hp_tf->set_text("HP " + std::to_string(health()));
}

void Player::inject_eliminations_amount_caption(LEti::Text_Field *_eliminations_amount_tf)
{
    m_eliminations_amount_tf = _eliminations_amount_tf;
    m_eliminations_amount_tf->set_text("Eliminations " + std::to_string(m_eliminations_amount));
}



void Player::reconstruct()
{
    Block::Size arrays_sizes;

    unsigned int occupied_cells = 0;
    for(unsigned int x=0; x<current_structure().width(); ++x)
    {
        for(unsigned int y=0; y<current_structure().height(); ++y)
        {
            const Block* material = current_structure().block(x, y).material;

            if(!material)
                continue;

            ++occupied_cells;
            arrays_sizes += material->get_size();
        }
    }

    if(occupied_cells == 0)
        return;

    float* coords = new float[arrays_sizes.coords];
    float* colors = new float[arrays_sizes.colors];
    float* t_coords = new float[arrays_sizes.texture_coords];
    float* phys_coords = new float[arrays_sizes.phys_coords];
    bool* collision_permissions = new bool[arrays_sizes.collision_permissions];
    float* masses = new float[arrays_sizes.masses];

    unsigned int polygons_amount = arrays_sizes.masses; //  one mass value per polygon
    unsigned int* block_indices_x = new unsigned int[polygons_amount];
    unsigned int* block_indices_y = new unsigned int[polygons_amount];

    float coords_scale = 1.0f / (float)(current_structure().width() > current_structure().height() ? current_structure().width() : current_structure().height());
    float single_block_mass_scale = 1.0f / (float)(current_structure().width() * current_structure().height());

    Block::Size offsets;
    for(unsigned int x=0; x<current_structure().width(); ++x)
    {
        for(unsigned int y=0; y<current_structure().height(); ++y)
        {
            const Space_Ship_Structure::Block_Data& block_data = current_structure().block(x, y);
            const Block* material = block_data.material;

            if(!material)
                continue;

            glm::vec3 coords_stride(coords_scale * (float)x, coords_scale * (float)y, 0.0f);

            material->copy_coords(coords, offsets.coords, coords_stride, block_data.angle, coords_scale);
            material->copy_colors(colors, offsets.colors);
            material->copy_texture_coords(t_coords, offsets.texture_coords);
            material->copy_phys_coords(phys_coords, offsets.phys_coords, coords_stride, block_data.angle, coords_scale);
            material->copy_collision_permissions(collision_permissions, offsets.collision_permissions);
            material->copy_masses(masses, offsets.masses, single_block_mass_scale);

            for(unsigned int i = offsets.masses; i < offsets.masses + material->get_size().masses; ++i)
            {
                block_indices_x[i] = x;
                block_indices_y[i] = y;
            }

            offsets += material->get_size();
        }
    }

    LEti::Default_Draw_Module_2D* dm = (LEti::Default_Draw_Module_2D*)draw_module();
    Physics_Module__Space_Ship* pm = (Physics_Module__Space_Ship*)physics_module();

    dm->init_vertices(coords, arrays_sizes.coords);
    dm->init_colors(colors, arrays_sizes.colors);
    dm->init_texture(dm->texture().get_picture(), t_coords, arrays_sizes.texture_coords);

    pm->init_physical_model();
    pm->setup_base_data(phys_coords, arrays_sizes.phys_coords, collision_permissions);
    pm->set_masses(masses);
    pm->set_block_indices(block_indices_x, block_indices_y);
    pm->init_prev_state();

    delete[] coords;
    delete[] colors;
    delete[] t_coords;
    delete[] phys_coords;
    delete[] collision_permissions;
    delete[] masses;

    delete[] block_indices_x;
    delete[] block_indices_y;

    ((LEti::Physics_Module__Rigid_Body_2D*)physics_module())->align_to_center_of_mass(draw_module());
}



void Player::M_process_hit_block(unsigned int _x, unsigned int _y)
{
    m_current_structure.block(_x, _y).health -= 1;

    if(m_current_structure.block(_x, _y).health > 0)
        return;

    if(_x == m_current_structure.cabin_x() && _y == m_current_structure.cabin_y())
    {
        m_cabin_is_broken = true;
        return;
    }

    m_current_structure.place_block(_x, _y, {0.0f, 0, nullptr});

    reconstruct();

    update(0.0f);
    update_previous_state();
}



void Player::temp_apply_simple_input()
{
    glm::vec3 impulse = {0.0f, 0.0f, 0.0f};

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
    {
        impulse.x -= acceleration();
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
    {
        impulse.x += acceleration();
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
    {
        impulse.y += acceleration();
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
    {
        impulse.y -= acceleration();
    }
    M_get_physics_module()->set_velocity(impulse);

    glm::vec3 pos_to_cursor = get_pos() - m_camera->convert_window_coords({LEti::Window_Controller::get_cursor_position().x, LEti::Window_Controller::get_cursor_position().y, 0.0f});
    LEti::Math::shrink_vector_to_1(pos_to_cursor);
    set_rotation_angle(LEti::Math::PI - acos(pos_to_cursor.x));
    if(pos_to_cursor.y > 0.0f)
        set_rotation_angle(LEti::Math::DOUBLE_PI - get_rotation_angle());

    m_shoot_timer.update(LEti::Event_Controller::get_dt());

    if((LEti::Event_Controller::is_key_down(GLFW_KEY_SPACE) || LEti::Event_Controller::is_mouse_button_down(GLFW_MOUSE_BUTTON_1)) && !m_shoot_timer.is_active())
    {
        M_shoot();
        m_shoot_timer.start(0.6f);
    }
}

void Player::apply_input()
{
//    temp_apply_simple_input();
//    return;

    bool has_rotational_input = false;

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_A))
    {
        M_get_physics_module()->apply_rotation(rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(M_get_physics_module()->angular_velocity() > max_rotation_speed())
            M_get_physics_module()->set_angular_velocity(max_rotation_speed());
        has_rotational_input = true;

//        move(glm::vec3{-300.0f, 0.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_D))
    {
        M_get_physics_module()->apply_rotation(-rotation_acceleration() * LEti::Event_Controller::get_dt());
        if(M_get_physics_module()->angular_velocity() < -max_rotation_speed())
            M_get_physics_module()->set_angular_velocity(-max_rotation_speed());
        has_rotational_input = true;

//        move(glm::vec3{300.0f, 0.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }

    if(!has_rotational_input && !LEti::Math::floats_are_equal(M_get_physics_module()->angular_velocity(), 0.0f))
    {
        float multiplier = M_get_physics_module()->angular_velocity() < 0.0f ? 1.0f : -1.0f;

        M_get_physics_module()->set_angular_velocity(M_get_physics_module()->angular_velocity() + (rotation_acceleration() * multiplier * LEti::Event_Controller::get_dt()));

        if(fabs(M_get_physics_module()->angular_velocity()) < rotation_acceleration() * LEti::Event_Controller::get_dt())
            M_get_physics_module()->set_angular_velocity(0.0f);
    }

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_W))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();
        M_get_physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(M_get_physics_module()->velocity());
        if(speed > max_speed())
            M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));

//        move(glm::vec3{0.0f, 300.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }
    if(LEti::Event_Controller::is_key_down(GLFW_KEY_S))
    {
        glm::vec3 impulse = LEti::Math::rotate_vector({-acceleration(), 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle()) * LEti::Event_Controller::get_dt();
        M_get_physics_module()->apply_linear_impulse(impulse);

        float speed = LEti::Math::vector_length(M_get_physics_module()->velocity());
        if(speed > max_speed())
            M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));

//        move(glm::vec3{0.0f, -300.0f, 0.0f} * LEti::Event_Controller::get_dt());
    }

    m_shoot_timer.update(LEti::Event_Controller::get_dt());

    if(LEti::Event_Controller::is_key_down(GLFW_KEY_SPACE) && !m_shoot_timer.is_active())
    {
        M_shoot();
        m_shoot_timer.start(0.6f);
    }

}

void Player::update(float _ratio)
{
    Space_Ship::update(_ratio);

    m_camera->set_position(get_pos());
}

void Player::on_other_entity_death(const Entity *_entity_to_delete)
{
    Space_Ship::on_other_entity_death(_entity_to_delete);
}

void Player::on_collision(const LEti::Intersection_Data& _id)
{
    Space_Ship::on_collision(_id);

    set_health(999);

    if(health() > 0)
        m_player_hp_tf->set_text("HP " + std::to_string(health()));
    else
        m_player_hp_tf->set_text(" ");

    unsigned int hit_polygon_index = (_id.first == this ? _id.first_collided_polygon_index : _id.second_collided_polygon_index);

    Polygon__Space_Ship* hit_polygon = (Polygon__Space_Ship*)(physics_module()->get_physical_model()->get_polygon(hit_polygon_index));

    unsigned int hit_index_x = hit_polygon->block_index_x();
    unsigned int hit_index_y = hit_polygon->block_index_y();

    M_process_hit_block(hit_index_x, hit_index_y);
}

void Player::on_death()
{
    Space_Ship::on_death();

    m_player_controller->notify_about_player_death();
}


bool Player::should_be_destroyed() const
{
    return m_cabin_is_broken;
}



void Player::set_eliminations_amount(unsigned int _amount)
{
    m_eliminations_amount = _amount;
    m_eliminations_amount_tf->set_text("Eliminations " + std::to_string(m_eliminations_amount));
}
