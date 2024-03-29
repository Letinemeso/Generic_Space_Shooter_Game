#ifndef BLOCK_CONTROLLER_H
#define BLOCK_CONTROLLER_H

#include <Data_Structures/Map.h>
#include <Data_Structures/Binary_Heap.h>
#include <Data_Structures/Vector.h>

#include <Variable_Base.h>

#include <Game_World/Player.h>

namespace GSSG
{

    class Block : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    public:
        struct Size
        {
        public:
            unsigned int coords = 0;
            unsigned int colors = 0;
            unsigned int texture_coords = 0;
            unsigned int phys_coords = 0;
            unsigned int collision_permissions = 0;
            unsigned int masses = 0;

        public:
            inline void operator+=(const Size& _other)
            {
                coords += _other.coords;
                colors += _other.colors;
                texture_coords += _other.texture_coords;
                phys_coords += _other.phys_coords;
                collision_permissions += _other.collision_permissions;
                masses += _other.masses;
            }

            inline void operator=(const Size& _other)
            {
                coords = _other.coords;
                colors = _other.colors;
                texture_coords = _other.texture_coords;
                phys_coords = _other.phys_coords;
                collision_permissions = _other.collision_permissions;
                masses = _other.masses;
            }

            inline void nullify()
            {
                coords = 0;
                colors = 0;
                texture_coords = 0;
                phys_coords = 0;
                collision_permissions = 0;
                masses = 0;
            }

        };

    private:
        unsigned int id = 0;

        float* coords = nullptr;
        float* colors = nullptr;
        float* texture_coords = nullptr;
        float* phys_coords = nullptr;
        bool* collision_permissions = nullptr;
        float* masses = nullptr;
        bool* connection_permissions = nullptr;

        unsigned int block_health = 0;

        Size size;

    public:
        void on_values_assigned() override;

    public:
        Block();
        Block(const Block& _other);
        Block(Block&& _other);

        void operator=(const Block& _other);

        virtual ~Block();

    protected:
        template<typename T>
        static void M_copy_array(const T* _what, T* _where, unsigned int _offset, unsigned int _size)
            { for(unsigned int i = 0; i < _size; ++i) _where[i + _offset] = _what[i]; }

    public:
        void copy_coords(float* _where, unsigned int _offset, const glm::vec3& _stride, float _rotation, float _scale) const;
        void copy_colors(float* _where, unsigned int _offset) const;
        void copy_texture_coords(float* _where, unsigned int _offset) const;
        void copy_phys_coords(float* _where, unsigned int _offset, const glm::vec3& _stride, float _rotation, float _scale) const;
        void copy_collision_permissions(bool* _where, unsigned int _offset) const;
        void copy_masses(float* _where, unsigned int _offset, float _scale) const;

        inline unsigned int get_id() const { return id; }
        inline const float* get_coords() const { return coords; }
        inline const float* get_colors() const { return colors; }
        inline const float* get_texture_coords() const { return texture_coords; }
        inline const float* get_phys_coords() const { return phys_coords; }
        inline const bool* get_collision_permissions() const { return collision_permissions; }
        inline const float* get_masses() const { return masses; }
        inline const bool* get_connection_permissions() const { return connection_permissions; }

        inline unsigned int get_block_health() const { return block_health; }

        inline const Size& get_size() const { return size; }

    public:
        virtual void apply_block_effect_on_construction(Space_Ship* _space_ship) const;
        virtual void apply_block_effect(Space_Ship* _space_ship, float _block_rotation, const glm::vec3& _block_position, float _dt) const;

    };

    class Cabin : public Block
    {
    public:
        DECLARE_VARIABLE;

    private:
        float shoot_delay = 1.0f;

    public:
        void apply_block_effect_on_construction(Space_Ship* _space_ship) const override;
        void apply_block_effect(Space_Ship* _space_ship, float _block_rotation, const glm::vec3& _block_position, float _dt) const override;

    };

    class Engine : public Block
    {
    public:
        DECLARE_VARIABLE;

    private:
        float acceleration = 1.0f;

    public:
        void apply_block_effect_on_construction(Space_Ship* _space_ship) const override;
        void apply_block_effect(Space_Ship* _space_ship, float _block_rotation, const glm::vec3& _block_position, float _dt) const override;

    };

    class Block_Controller final
    {
    private:
        LDS::Map<std::string, LST::Function<Block*()>> m_block_allocators;
        LDS::Map<unsigned int, Block*> m_blocks_map;
        LDS::Binary_Heap<unsigned int> m_ids_sorted;

    public:
        Block_Controller();
        ~Block_Controller();

    public:
        void clear();
        void init(const LV::MDL_Variable_Stub& _blocks);

    public:
        bool block_exists(unsigned int _id) const;
        const Block& get_block(unsigned int _id) const;
        const LDS::Vector<unsigned int>& get_block_ids() const;

    };

}

#endif // BLOCK_CONTROLLER_H
