#ifndef SPACE_SHIP_STRUCTURE_H
#define SPACE_SHIP_STRUCTURE_H

#include <Variable_Base.h>

#include <Object_System/Builder_Stub.h>

#include <Graph/Graph.h>
#include <Graph/Pathfinder.h>

namespace GSSG
{

    class Block_Controller;
    class Block;

    class Space_Ship_Structure_Stub : public LEti::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        const Block_Controller* block_controller = nullptr;

    public:
        unsigned int width = 0;
        unsigned int height = 0;

        unsigned int* block_ids = nullptr;
        unsigned int* block_rotations = nullptr;
        unsigned int* key_bounds = nullptr;

    private:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    public:
        void on_values_assigned() override;
        ~Space_Ship_Structure_Stub();

    };

    class Space_Ship_Structure : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    public:
        struct Block_Data
        {
            unsigned int bound_key = 0;
            float angle = 0.0f;
            unsigned int health = 0;
            const Block* material = nullptr;
        };

    private:
        unsigned int m_width = 0, m_height = 0;
        Block_Data** m_blocks_data = nullptr;

        unsigned int m_cabin_x = 0xFFFFFFFF;
        unsigned int m_cabin_y = 0xFFFFFFFF;
        unsigned int m_cabin_graph_index = 0xFFFFFFFF;

    private:
        LGL::Graph m_graph;
        mutable LGL::Pathfinder m_block_connection_check;

    public:
        Space_Ship_Structure();
        Space_Ship_Structure(const Space_Ship_Structure& _other);
        void operator=(const Space_Ship_Structure& _other);
        Space_Ship_Structure(Space_Ship_Structure&& _other);
        void operator=(Space_Ship_Structure&& _other);
        ~Space_Ship_Structure();

        void clear();
        void resize(unsigned int _width, unsigned int _height);
        void reset_blocks();

    public:
        inline unsigned int width() const { return m_width; }
        inline unsigned int height() const { return m_height; }
        inline unsigned int cabin_x() const { return m_cabin_x; }
        inline unsigned int cabin_y() const { return m_cabin_y; }

        Block_Data& block(unsigned int _x, unsigned int _y);
        const Block_Data& block(unsigned int _x, unsigned int _y) const;

    private:
        unsigned int M_calculate_graph_node_index(unsigned int _x, unsigned int _y) const;
        std::pair<unsigned int, unsigned int> M_calculate_grid_index(unsigned int _graph_index) const;

        unsigned int M_get_connection_permission_index(unsigned int _expected_index, float _rotation_angle) const;

        void M_set_cabin_index(unsigned int _x, unsigned int _y);
        void M_update_connections(unsigned int _x, unsigned int _y);
        void M_reset_not_connected_blocks();

    public:
        bool block_can_be_placed(unsigned int _x, unsigned int _y, const Block_Data& _block) const;
        bool place_block(unsigned int _x, unsigned int _y, const Block_Data& _block);

    public:
        void place_block_force(unsigned int _x, unsigned int _y, const Block_Data& _block);
        bool block_placement_is_valid() const;

    };

}

#endif // SPACE_SHIP_STRUCTURE_H
