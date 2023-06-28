#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <Graph/Graph.h>
#include <Graph/Pathfinder.h>

#include <Game_World/Entity_Manager.h>
#include <Game_World/Entity.h>
#include <Game_World/Projectile.h>


namespace GSSG
{

    class Block;

    class Space_Ship_Structure
    {
    public:
        struct Block_Data
        {
            float angle = 0.0f;
            unsigned int health = 0;
            const Block* material = nullptr;
        };

    private:
        unsigned int m_width = 0.0f, m_height = 0.0f;
        Block_Data** m_blocks_data = nullptr;

        unsigned int m_cabin_x = 0xFFFFFFFF;
        unsigned int m_cabin_y = 0xFFFFFFFF;
        unsigned int m_cabin_graph_index = 0xFFFFFFFF;

    private:
        LGL::Graph m_graph;
        LGL::Pathfinder m_block_connection_check;

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

    };

    class Space_Ship : public Entity
    {
    public:
        DECLARE_VARIABLE;

    protected:
        Entity_Manager* m_entity_manager = nullptr;

    private:
        const LEti::Object_2D_Stub* m_projectile_stub = nullptr;

        float m_acceleration = 500.0f;
        float m_max_speed = 250.0f;
        float m_rotation_acceleration = LEti::Math::DOUBLE_PI;
        float m_max_rotation_speed = LEti::Math::PI;

    public:
        Space_Ship();
        ~Space_Ship();

    public:
        inline void inject_entity_manager(Entity_Manager* _entity_manager) { m_entity_manager = _entity_manager; }

        inline void set_projectile_stub(const LEti::Object_2D_Stub* _projectile_stub) { m_projectile_stub = _projectile_stub; }

    public:
        inline float acceleration() const { return m_acceleration; }
        inline float max_speed() const { return m_max_speed; }
        inline float rotation_acceleration() const { return m_rotation_acceleration; }
        inline float max_rotation_speed() const { return m_max_rotation_speed; }

    protected:
        void M_shoot();

    public:
//        bool should_be_destroyed() const override;

    public:
        void update(float _ratio = 1.0f) override;

    };

}


#endif // SPACE_SHIP_H
