#ifndef BLOCK_CONTROLLER_H
#define BLOCK_CONTROLLER_H

#include <Data_Structures/Map.h>
#include <Data_Structures/Binary_Heap.h>
#include <Data_Structures/Vector.h>

#include <Variable_Base.h>

#include <Player.h>

namespace GSSG
{

    class Block final : public LV::Variable_Base
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

        Size size;

    protected:
        void M_on_values_assigned() override;

    public:
        Block();
        Block(const Block& _other);
        Block(Block&& _other);

        void operator=(const Block& _other);

        ~Block();

    private:
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

        inline int get_id() const { return id; }
        inline const float* get_coords() const { return coords; }
        inline const float* get_colors() const { return colors; }
        inline const float* get_texture_coords() const { return texture_coords; }
        inline const float* get_phys_coords() const { return phys_coords; }
        inline const bool* get_collision_permissions() const { return collision_permissions; }
        inline const float* get_masses() const { return masses; }

        inline const Size& get_size() const { return size; }

    };

    class Block_Controller
    {
    private:
        LDS::Map<unsigned int, Block> m_blocks_map;
        LDS::Binary_Heap<unsigned int> m_ids_sorted;

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
