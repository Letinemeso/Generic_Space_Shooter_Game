#include <Edit_Mode/Block_Controller.h>

using namespace GSSG;


INIT_FIELDS(GSSG::Block, LV::Variable_Base)

ADD_FIELD(unsigned int, id);

ADD_FIELD(float*, coords);
ADD_FIELD(float*, colors);
ADD_FIELD(float*, texture_coords);
ADD_FIELD(float*, phys_coords);
ADD_FIELD(bool*, collision_permissions);
ADD_FIELD(float*, masses);

ADD_FIELD(unsigned int, size.coords);
ADD_FIELD(unsigned int, size.colors);
ADD_FIELD(unsigned int, size.texture_coords);
ADD_FIELD(unsigned int, size.phys_coords);

FIELDS_END


void Block::M_on_values_assigned()
{
    LV::Variable_Base::M_on_values_assigned();
    size.collision_permissions = size.phys_coords / 3;
    size.masses = size.phys_coords / 9;
}



Block::Block()
{

}

Block::Block(const Block& _other)
{
    id = _other.id;

    size = _other.size;

    L_ASSERT(size.coords > 0 && size.colors > 0 && size.texture_coords > 0 && size.phys_coords > 0 && size.collision_permissions > 0 && size.masses > 0);

    coords = new float[size.coords];
    colors = new float[size.colors];
    texture_coords = new float[size.texture_coords];
    phys_coords = new float[size.phys_coords];
    collision_permissions = new bool[size.collision_permissions];
    masses = new float[size.masses];

    M_copy_array(_other.coords, coords, 0, size.coords);
    M_copy_array(_other.colors, colors, 0, size.colors);
    M_copy_array(_other.texture_coords, texture_coords, 0, size.texture_coords);
    M_copy_array(_other.phys_coords, phys_coords, 0, size.phys_coords);
    M_copy_array(_other.collision_permissions, collision_permissions, 0, size.collision_permissions);
    M_copy_array(_other.masses, masses, 0, size.masses);
}

Block::Block(Block&& _other)
{
    id = _other.id;
    _other.id = 0;

    coords = _other.coords;
    _other.coords = nullptr;
    colors = _other.colors;
    _other.colors = nullptr;
    texture_coords = _other.texture_coords;
    _other.texture_coords = nullptr;
    phys_coords = _other.phys_coords;
    _other.phys_coords = nullptr;
    collision_permissions = _other.collision_permissions;
    _other.collision_permissions = nullptr;
    masses = _other.masses;
    _other.masses = nullptr;

    size = _other.size;
    _other.size.nullify();
}


void Block::operator=(const Block &_other)
{
    id = _other.id;

    size = _other.size;

    L_ASSERT(size.coords > 0 && size.colors > 0 && size.texture_coords > 0 && size.phys_coords > 0 && size.collision_permissions > 0 && size.masses > 0);

    coords = new float[size.coords];
    colors = new float[size.colors];
    texture_coords = new float[size.texture_coords];
    phys_coords = new float[size.phys_coords];
    collision_permissions = new bool[size.collision_permissions];
    masses = new float[size.masses];

    M_copy_array(_other.coords, coords, 0, size.coords);
    M_copy_array(_other.colors, colors, 0, size.colors);
    M_copy_array(_other.texture_coords, texture_coords, 0, size.texture_coords);
    M_copy_array(_other.phys_coords, phys_coords, 0, size.phys_coords);
    M_copy_array(_other.collision_permissions, collision_permissions, 0, size.collision_permissions);
    M_copy_array(_other.masses, masses, 0, size.masses);
}


Block::~Block()
{
    delete[] coords;
    delete[] colors;
    delete[] texture_coords;
    delete[] phys_coords;
    delete[] collision_permissions;
    delete[] masses;
}



void Block::copy_coords(float *_where, unsigned int _offset) const
{
    L_ASSERT(coords && size.coords > 0);
    M_copy_array(coords, _where, _offset, size.coords);
}

void Block::copy_colors(float *_where, unsigned int _offset) const
{
    L_ASSERT(colors && size.colors > 0);
    M_copy_array(colors, _where, _offset, size.colors);
}

void Block::copy_texture_coords(float *_where, unsigned int _offset) const
{
    L_ASSERT(texture_coords && size.texture_coords > 0);
    M_copy_array(texture_coords, _where, _offset, size.texture_coords);
}

void Block::copy_phys_coords(float *_where, unsigned int _offset) const
{
    L_ASSERT(phys_coords && size.phys_coords > 0);
    M_copy_array(phys_coords, _where, _offset, size.phys_coords);
}

void Block::copy_collision_permissions(bool *_where, unsigned int _offset) const
{
    L_ASSERT(collision_permissions && size.collision_permissions > 0);
    M_copy_array(collision_permissions, _where, _offset, size.collision_permissions);
}

void Block::copy_masses(float *_where, unsigned int _offset) const
{
    L_ASSERT(masses && size.masses > 0);
    M_copy_array(masses, _where, _offset, size.masses);
}



//  Block_Controller

void Block_Controller::clear()
{
    m_blocks_map.clear();
}

void Block_Controller::init(const LV::MDL_Variable_Stub &_blocks)
{
    clear();

    LDS::Vector<unsigned int> ids;
    ids.resize(_blocks.childs.size());

    for(auto it = _blocks.childs.iterator(); !it.end_reached(); ++it)
    {
        Block block;
        block.assign_values(*it);
        unsigned int id = block.get_id();
        m_blocks_map.insert(id, (Block&&)block);
        ids.push(id);
    }

    m_ids_sorted.build_from_raw((LDS::Vector<unsigned int>&&)ids);
    m_ids_sorted.sort();
}



bool Block_Controller::block_exists(unsigned int _id) const
{
    auto it = m_blocks_map.find(_id);
    return it.is_ok();
}

const Block& Block_Controller::get_block(unsigned int _id) const
{
    auto it = m_blocks_map.find(_id);
    L_ASSERT(it.is_ok());
    return *it;
}

const LDS::Vector<unsigned int>& Block_Controller::get_block_ids() const
{
    return m_ids_sorted.array();
}
