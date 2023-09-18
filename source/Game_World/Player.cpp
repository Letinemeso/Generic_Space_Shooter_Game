#include <Game_World/Player.h>

#include <Game_World/Player_Controller.h>
#include <Edit_Mode/Block_Controller.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Player_Stub, LV::Builder_Stub)
FIELDS_END



LV::Variable_Base* Player_Stub::M_construct_product() const
{
    return new Player;
}

void Player_Stub::M_init_constructed_product(LV::Variable_Base *_product) const
{
    Player* product = (Player*)_product;

    LR::Default_Draw_Module_2D* dm = new LR::Default_Draw_Module_2D;
    dm->assign_values({});
    dm->on_values_assigned();
    dm->set_texture(picture);
    dm->set_renderer(renderer);
    Physics_Module__Space_Ship* pm = new Physics_Module__Space_Ship;
    pm->assign_values({});
    pm->on_values_assigned();

    pm->set_on_alignment_func([pm, dm]()
    {
        dm->move_raw(-pm->calculate_raw_center_of_mass());
    });

    product->on_reconstruction = [dm](const float* _coords, unsigned int _coords_amount, const float* _colors, unsigned int _colors_amount, const float* _t_coords, unsigned int _t_coords_amount)
    {
        dm->init_vertices(_coords, _coords_amount);
        dm->init_colors(_colors, _colors_amount);
        dm->init_texture(dm->texture().get_picture(), _t_coords, _t_coords_amount);
    };

    product->set_physics_module(pm);

    product->set_structure(structure);

    product->inject_camera(camera);
    product->inject_effects_controller(effects_controller);
    product->set_on_death_effect(on_death_effect);

    product->reconstruct();

    product->current_state().set_scale({100.0f, 100.0f, 1.0f});
    product->current_state().set_position({0.0f, 0.0f, 0.0f});
    product->current_state().set_rotation({0.0f, 0.0f, LEti::Math::HALF_PI});

    pm->set_owner(product);

    product->add_module(pm);
    product->add_module(dm);

    product->update(0.0f);
    product->update_previous_state();
}



INIT_FIELDS(GSSG::Player, GSSG::Space_Ship)
FIELDS_END



Player::Player()
{

}

Player::~Player()
{

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
//    float single_block_mass_scale = 1.0f / (float)(current_structure().width() * current_structure().height());

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
            material->copy_masses(masses, offsets.masses, 1.0f);

            for(unsigned int i = offsets.masses; i < offsets.masses + material->get_size().masses; ++i)
            {
                block_indices_x[i] = x;
                block_indices_y[i] = y;
            }

            offsets += material->get_size();
        }
    }

    on_reconstruction(coords, arrays_sizes.coords, colors, arrays_sizes.colors, t_coords, arrays_sizes.texture_coords);

//    LR::Default_Draw_Module_2D* dm = (LR::Default_Draw_Module_2D*)draw_module();
    Physics_Module__Space_Ship* pm = (Physics_Module__Space_Ship*)physics_module();

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

    m_block_pos_offset = pm->calculate_raw_center_of_mass();

    pm->align_to_center_of_mass();

    for(unsigned int x=0; x<current_structure().width(); ++x)
    {
        for(unsigned int y=0; y<current_structure().height(); ++y)
        {
            const Space_Ship_Structure::Block_Data& block_data = current_structure().block(x, y);
            const Block* material = block_data.material;

            if(!material)
                continue;

            material->apply_block_effect_on_construction(this);
        }
    }
}



glm::vec3 Player::M_calculate_block_global_pos(unsigned int _x, unsigned int _y) const
{
    glm::vec3 block_scale(1.0f / m_current_structure.width(), 1.0f / m_current_structure.height(), 1.0f);
    glm::vec3 block_pos(block_scale.x * (float)_x, block_scale.y * (float)_y, 0.0f);
//    block_pos -= glm::vec3(0.5f, 0.5f, 0.0f);
    block_pos -= m_block_pos_offset;
    block_pos = (m_current_state.rotation_matrix() * m_current_state.scale_matrix()) * glm::vec4(block_pos, 1.0f);
    block_pos += current_state().position();

    return block_pos;
}

void Player::M_create_block_destruction_effect(unsigned int _x, unsigned int _y)
{
    glm::vec3 effect_scale(1.0f / m_current_structure.width(), 1.0f / m_current_structure.height(), 1.0f);
    effect_scale = current_state().scale() * ( 1.0f / (float)(m_current_structure.width() > m_current_structure.height() ? m_current_structure.height() : m_current_structure.width()) );

    Visual_Effect* effect = (Visual_Effect*)m_on_death_effect->construct();
    effect->current_state().set_scale(effect_scale);
    effect->current_state().set_position(M_calculate_block_global_pos(_x, _y));
    effect->current_state().set_rotation(current_state().rotation());
    m_effects_controller->add_object(effect);
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

    M_create_block_destruction_effect(_x, _y);

    Space_Ship_Structure::Block_Data block_data;
    m_current_structure.place_block(_x, _y, block_data);

    reconstruct();

    update(0.0f);
    update_previous_state();
}



void Player::apply_input(float _dt)
{
    for(unsigned int x=0; x<current_structure().width(); ++x)
    {
        for(unsigned int y=0; y<current_structure().height(); ++y)
        {
            const Space_Ship_Structure::Block_Data& block_data = current_structure().block(x, y);
            const Block* material = block_data.material;

            if(!material)
                continue;

            if(!LR::Window_Controller::is_key_down(block_data.bound_key))
                continue;

            material->apply_block_effect(this, m_current_structure.block(x, y).angle, M_calculate_block_global_pos(x, y), _dt);
        }
    }

    float rotation_ratio = fabs(physics_module()->angular_velocity() / max_rotation_speed());
    float velocity_ratio = fabs(LEti::Math::vector_length(physics_module()->velocity()) / max_speed());

    if(rotation_ratio > 1.0f)
        physics_module()->set_angular_velocity(physics_module()->angular_velocity() / rotation_ratio);

    if(velocity_ratio > 1.0f)
        physics_module()->set_velocity(physics_module()->velocity() / velocity_ratio);

    m_shoot_timer.update(_dt);

    if(LR::Window_Controller::is_key_down(GLFW_KEY_SPACE) && !m_shoot_timer.is_active())
    {
        M_shoot();
        m_shoot_timer.start(0.6f);
    }

}

void Player::update(float _dt)
{
    Space_Ship::update(_dt);

    m_camera->set_position(current_state().position());
}

void Player::on_other_entity_death(const Entity *_entity_to_delete)
{
    Space_Ship::on_other_entity_death(_entity_to_delete);
}

void Player::on_collision(const LPhys::Intersection_Data& _id)
{
    Space_Ship::on_collision(_id);

    unsigned int hit_polygon_index = (_id.first == physics_module() ? _id.first_collided_polygon_index : _id.second_collided_polygon_index);

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
    m_eliminations_amount_tf->text = "Eliminations " + std::to_string(m_eliminations_amount);
}
