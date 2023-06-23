#include <Game_World/Space_Ship.h>

#include <Edit_Mode/Block_Controller.h>

using namespace GSSG;


Space_Ship_Structure::Space_Ship_Structure()
{

}

Space_Ship_Structure::Space_Ship_Structure(const Space_Ship_Structure& _other)
{
    resize(_other.width(), _other.height());

    for(unsigned int x=0; x<width(); ++x)
        for(unsigned int y=0; y<height(); ++y)
            m_blocks_data[x][y] = _other.m_blocks_data[x][y];

    m_cabin_x = _other.m_cabin_x;
    m_cabin_y = _other.m_cabin_y;
    m_cabin_graph_index = _other.m_cabin_graph_index;
}

void Space_Ship_Structure::operator=(const Space_Ship_Structure& _other)
{
    clear();
    resize(_other.width(), _other.height());

    for(unsigned int x=0; x<width(); ++x)
        for(unsigned int y=0; y<height(); ++y)
            m_blocks_data[x][y] = _other.m_blocks_data[x][y];

    m_cabin_x = _other.m_cabin_x;
    m_cabin_y = _other.m_cabin_y;
    m_cabin_graph_index = _other.m_cabin_graph_index;
}

Space_Ship_Structure::Space_Ship_Structure(Space_Ship_Structure&& _other)
{
    m_width = _other.m_width;
    _other.m_width = 0;
    m_height = _other.m_height;
    _other.m_height = 0;

    m_blocks_data = _other.m_blocks_data;
    _other.m_blocks_data = nullptr;

    m_cabin_x = _other.m_cabin_x;
    m_cabin_y = _other.m_cabin_y;
    m_cabin_graph_index = _other.m_cabin_graph_index;

    _other.M_set_cabin_index(0xFFFFFFFF, 0xFFFFFFFF);
}

void Space_Ship_Structure::operator=(Space_Ship_Structure&& _other)
{
    clear();

    m_width = _other.m_width;
    _other.m_width = 0;
    m_height = _other.m_height;
    _other.m_height = 0;

    m_blocks_data = _other.m_blocks_data;
    _other.m_blocks_data = nullptr;

    m_cabin_x = _other.m_cabin_x;
    m_cabin_y = _other.m_cabin_y;
    m_cabin_graph_index = _other.m_cabin_graph_index;

    _other.M_set_cabin_index(0xFFFFFFFF, 0xFFFFFFFF);
}

Space_Ship_Structure::~Space_Ship_Structure()
{
    clear();
}


void Space_Ship_Structure::clear()
{
    for(unsigned int i=0; i<width(); ++i)
        delete[] m_blocks_data[i];
    delete[] m_blocks_data;

    m_blocks_data = nullptr;

    m_width = 0;
    m_height = 0;

    m_graph.clear();

    M_set_cabin_index(0xFFFFFFFF, 0xFFFFFFFF);
}

void Space_Ship_Structure::resize(unsigned int _width, unsigned int _height)
{
    clear();

    m_width = _width;
    m_height = _height;

    m_blocks_data = new Block_Data*[m_width];
    for(unsigned int i=0; i<width(); ++i)
        m_blocks_data[i] = new Block_Data[m_height];

    m_graph.clear();
    m_graph.allocate_nodes(width() * height());
    m_block_connection_check.set_graph(&m_graph);
    m_block_connection_check.set_distance_calculation_func([this](unsigned int _first, unsigned int _second)
    {
        std::pair<unsigned int, unsigned int> first_index = M_calculate_grid_index(_first);
        std::pair<unsigned int, unsigned int> second_index = M_calculate_grid_index(_second);

        return abs((int)first_index.first - (int)second_index.first) + abs((int)first_index.second - (int)second_index.second);
    });
}

void Space_Ship_Structure::reset_blocks()
{
    for(unsigned int x = 0; x < m_width; ++x)
    {
        for(unsigned int y = 0; y < m_height; ++y)
        {
            m_blocks_data[x][y].angle = 0.0f;
            m_blocks_data[x][y].material = nullptr;
        }
    }

    m_graph.clear();
    m_graph.allocate_nodes(width() * height());

    M_set_cabin_index(0xFFFFFFFF, 0xFFFFFFFF);
}



Space_Ship_Structure::Block_Data& Space_Ship_Structure::block(unsigned int _x, unsigned int _y)
{
    L_ASSERT(_x < m_width && _y < m_height);

    return m_blocks_data[_x][_y];
}

const Space_Ship_Structure::Block_Data& Space_Ship_Structure::block(unsigned int _x, unsigned int _y) const
{
    L_ASSERT(_x < m_width && _y < m_height);

    return m_blocks_data[_x][_y];
}



unsigned int Space_Ship_Structure::M_calculate_graph_node_index(unsigned int _x, unsigned int _y) const
{
    return _x * m_height + _y;
}

std::pair<unsigned int, unsigned int> Space_Ship_Structure::M_calculate_grid_index(unsigned int _graph_index) const
{
    std::pair<unsigned int, unsigned int> result;

    result.first = _graph_index / height();
    result.second = _graph_index % height();

    return result;
}


unsigned int Space_Ship_Structure::M_get_connection_permission_index(unsigned int _expected_index, float _rotation_angle) const
{
    float full_quarters = fabs(_rotation_angle / LEti::Math::HALF_PI);
    unsigned int quarters = LEti::Math::float_to_int(full_quarters) % 4;

    if(_rotation_angle < 0.0f)
        quarters = abs(((int)quarters - 4) % 4);

    unsigned int result = _expected_index + 4 - quarters;
    if(result >= 4)
        result -= 4;

    return result;
}


void Space_Ship_Structure::M_set_cabin_index(unsigned int _x, unsigned int _y)
{
    m_cabin_x = _x;
    m_cabin_y = _y;

    if(_x == 0xFFFFFFFF || _y == 0xFFFFFFFF)
        m_cabin_graph_index = 0xFFFFFFFF;
    else
        m_cabin_graph_index = M_calculate_graph_node_index(_x, _y);
}

void Space_Ship_Structure::M_update_connections(unsigned int _x, unsigned int _y)
{
    unsigned int index = M_calculate_graph_node_index(_x, _y);

    auto update_neighbour = [this, index, _x, _y](unsigned int _other_x, unsigned _other_y)->void
    {
        const Block_Data& neighbour = m_blocks_data[_other_x][_other_y];
        unsigned int neighbour_index = M_calculate_graph_node_index(_other_x, _other_y);
        bool should_be_connected = neighbour.material != nullptr && neighbour.material != nullptr;

        unsigned int expected_connection_permission_index = 0;

        if(_x < _other_x)
            expected_connection_permission_index = 0;
        else if(_x > _other_x)
            expected_connection_permission_index = 2;
        else if(_y < _other_y)
            expected_connection_permission_index = 1;
        else if(_y > _other_y)
            expected_connection_permission_index = 3;

        unsigned int connection_permission_index = M_get_connection_permission_index(expected_connection_permission_index, m_blocks_data[_x][_y].angle);
        unsigned int neighbour_connection_permission_index = M_get_connection_permission_index((expected_connection_permission_index + 2) % 4, m_blocks_data[_other_x][_other_y].angle);

        if(!m_blocks_data[_x][_y].material || !neighbour.material)
            should_be_connected = false;
        else if(m_blocks_data[_x][_y].material->get_connection_permissions()[connection_permission_index] && neighbour.material->get_connection_permissions()[neighbour_connection_permission_index])
            should_be_connected = true;
        else
            should_be_connected = false;

        if(should_be_connected)
        {
            if(m_graph.distance_if_linked(index, neighbour_index) == 0)
                m_graph.link_nodes(index, neighbour_index, 1, true);
        }
        else
        {
            if(m_graph.distance_if_linked(index, neighbour_index) > 0)
                m_graph.unlink_nodes(index, neighbour_index, true);
        }
    };

    if(_x > 0)
        update_neighbour(_x - 1, _y);
    if(_x < m_width - 1)
        update_neighbour(_x + 1, _y);
    if(_y > 0)
        update_neighbour(_x, _y - 1);
    if(_y < m_height - 1)
        update_neighbour(_x, _y + 1);
}

void Space_Ship_Structure::M_reset_not_connected_blocks()
{
    for(unsigned int x = 0; x < m_width; ++x)
    {
        for(unsigned int y = 0; y < m_height; ++y)
        {
            Block_Data& block_data = m_blocks_data[x][y];

            if(block_data.material == nullptr || (x == m_cabin_x && y == m_cabin_y))
                continue;

            unsigned int current_graph_index = M_calculate_graph_node_index(x, y);

            m_block_connection_check.process(current_graph_index, m_cabin_graph_index);
            if(m_block_connection_check.path_result().size() > 0)
                continue;

            block_data.angle = 0.0f;
            block_data.material = nullptr;

            M_update_connections(x, y);
        }
    }
}



bool Space_Ship_Structure::block_can_be_placed(unsigned int _x, unsigned int _y, const Block_Data& _block) const
{
    if(_block.material == nullptr )
        return true;
    if(LV::cast_variable<Cabin>(_block.material) != nullptr)
        return true;
    if(m_cabin_graph_index == 0xFFFFFFFF)
        return false;
    if(m_cabin_x == _x && m_cabin_y == _y)
        return false;

    for(unsigned int i=0; i<4; ++i)
    {
        unsigned int resolution_index = M_get_connection_permission_index(i, _block.angle);

        int x = _x;
        int y = _y;

        if(i == 0)
            ++x;
        else if(i == 1)
            ++y;
        else if(i == 2)
            --x;
        else if(i == 3)
            --y;

        if(x < 0 || x >= (int)m_width || y < 0 || y >= (int)m_height)
            continue;

        const Block_Data& nearby_block = m_blocks_data[x][y];

        if(nearby_block.material == nullptr)
            continue;

        unsigned int expected_nearby_index = i + 2;
        if(expected_nearby_index >= 4)
            expected_nearby_index -= 4;
        unsigned int nearby_index = M_get_connection_permission_index(expected_nearby_index, nearby_block.angle);

        if(_block.material->get_connection_permissions()[resolution_index] && nearby_block.material->get_connection_permissions()[nearby_index])
            return true;
    }

    return false;
}

bool Space_Ship_Structure::place_block(unsigned int _x, unsigned int _y, const Block_Data &_block)
{
    if(!block_can_be_placed(_x, _y, _block))
        return false;

    if(LV::cast_variable<Cabin>(_block.material) != nullptr)
    {
        reset_blocks();
        M_set_cabin_index(_x, _y);
    }

    m_blocks_data[_x][_y] = _block;

    M_update_connections(_x, _y);

    M_reset_not_connected_blocks();

    return true;
}





INIT_FIELDS(GSSG::Space_Ship, GSSG::Entity);
FIELDS_END;


Space_Ship::Space_Ship()
{

}

Space_Ship::~Space_Ship()
{

}



void Space_Ship::M_shoot()
{
    L_ASSERT(m_projectile_stub);

    Projectile* projectile = (Projectile*)m_projectile_stub->construct();
    LEti::Physics_Module__Rigid_Body_2D* pm = (LEti::Physics_Module__Rigid_Body_2D*)projectile->physics_module();

    glm::vec3 projectile_impulse = LEti::Math::rotate_vector({500.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, get_rotation_angle());

    projectile->set_pos(get_pos());
    projectile->set_rotation_angle(get_rotation_angle());
    projectile->set_parent(this);
    projectile->set_lifetime(1.5f);
    pm->set_velocity(projectile_impulse);

    projectile->update_previous_state();
    projectile->update(0.0f);

    m_entity_manager->add_entity(projectile);
return;
    M_get_physics_module()->apply_linear_impulse(-projectile_impulse * (pm->mass() / M_get_physics_module()->mass()));
    if(LEti::Math::vector_length(M_get_physics_module()->velocity()) > max_speed())
        M_get_physics_module()->set_velocity(LEti::Math::extend_vector_to_length(M_get_physics_module()->velocity(), max_speed()));
}



void Space_Ship::update(float _ratio)
{
    Entity::update(_ratio);
}
