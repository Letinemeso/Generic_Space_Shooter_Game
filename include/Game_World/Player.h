#ifndef PLAYER_H
#define PLAYER_H


#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>

#include <Event_Controller/Event_Controller.h>
#include <Object_System/Text_Field.h>

#include <Game_World/Enemy_Generator.h>
#include <Game_World/Entity_Manager.h>
#include <Game_World/Entity.h>
#include <Game_World/Projectile.h>
#include <Game_World/Space_Ship.h>
#include <Game_World/Physics_Module__Space_Ship.h>
#include <Game_World/Visual_Effect.h>


namespace GSSG
{

    class Player;

    class Player_Stub : public LV::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        Space_Ship_Structure structure;
        Effects_Controller* effects_controller = nullptr;
        const Visual_Effect_Stub* on_death_effect = nullptr;
        const LR::Picture* picture = nullptr;
        LR::Camera_2D* camera = nullptr;
        LR::Renderer* renderer = nullptr;

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base *_product) const override;

    };



    class Player_Controller;

    class Player : public GSSG::Space_Ship
    {
    public:
        DECLARE_VARIABLE;

    private:
        Space_Ship_Structure m_initial_structure;
        Space_Ship_Structure m_current_structure;

        bool m_cabin_is_broken = false;

    private:
        LR::Camera_2D* m_camera = nullptr;
        Player_Controller* m_player_controller = nullptr;

        LST::Timer m_shoot_timer;

        LEti::Text_Field* m_player_hp_tf = nullptr;

    private:
        LEti::Text_Field* m_eliminations_amount_tf = nullptr;
        unsigned int m_eliminations_amount = 0;

    private:
        glm::vec3 m_block_pos_offset;

    public:
        Player();
        ~Player();

    public:
        inline void inject_camera(LR::Camera_2D* _camera) { m_camera = _camera; }
        inline void inject_player_controller(Player_Controller* _player_controller) { m_player_controller = _player_controller; }
        void inject_player_hp_caption(LEti::Text_Field* _player_hp_tf);
        void inject_eliminations_amount_caption(LEti::Text_Field* _eliminations_amount_tf);

    public:
        inline void set_structure(const Space_Ship_Structure& _structure) { m_initial_structure = _structure; m_current_structure = _structure; }
        inline void set_structure(Space_Ship_Structure&& _structure) { m_initial_structure = (Space_Ship_Structure&&)_structure; m_current_structure = m_initial_structure; }

        inline const Space_Ship_Structure& initial_structure() const { return m_initial_structure; }
        inline const Space_Ship_Structure& current_structure() const { return m_current_structure; }

        LST::Function<void(const float*, unsigned int, const float*, unsigned int, const float*, unsigned int)> on_reconstruction;

        void reconstruct();

    private:
        glm::vec3 M_calculate_block_global_pos(unsigned int _x, unsigned int _y) const;
        void M_create_block_destruction_effect(unsigned int _x, unsigned int _y);
        void M_process_hit_block(unsigned int _x, unsigned int _y);

    public:
        void kill_force() { m_cabin_is_broken = true; }
        void temp_apply_simple_input();
        void apply_input() override;
        void update() override;
        void on_other_entity_death(const Entity* _entity_to_delete) override;
        void on_collision(const LPhys::Intersection_Data& _id) override;
        void on_death() override;

        bool should_be_destroyed() const override;

    public:
        void set_eliminations_amount(unsigned int _amount);

    public:
        inline unsigned int eliminations_amount() const { return m_eliminations_amount; }

    };

}


#endif // PLAYER_H
