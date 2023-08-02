#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <Data_Structures/List.h>

#include <Renderer/Renderer.h>
#include <Physics/Collision_Detector_2D.h>

#include <Camera/Camera_2D.h>

#include <Game_World/Entity.h>


namespace GSSG
{

    class Entity_Manager
    {
    private:
        LEti::Collision_Detector_2D* m_collision_detector = nullptr;
        const LR::Renderer* m_renderer = nullptr;
        const LR::Camera_2D* m_camera = nullptr;

    private:
        LDS::List<Entity*> m_entities;

    private:
        float m_max_distance_from_view_pos = 1000.0f;

    public:
        Entity_Manager();
        ~Entity_Manager();

    public:
        inline void inject_collision_detector(LEti::Collision_Detector_2D* _collision_detector) { m_collision_detector = _collision_detector; }
        inline void inject_renderer(LR::Renderer* _renderer) { m_renderer = _renderer; }
        inline void inject_camera(LR::Camera_2D* _camera) { m_camera = _camera; }

        inline void set_max_distance_from_view_pos(float _max) { m_max_distance_from_view_pos = _max; }

    private:
        void M_notify_other_entities_about_death(Entity* _entity);

    public:
        void add_entity(Entity* _entity);

    public:
        inline const LDS::List<Entity*>& registred_entities() const { return m_entities; }

    public:
        void update_entities_prev_state() const;
        void update_entities(float _ratio = 1.0f) const;
        void apply_entities_input() const;

        void remove_dead_entities();

        void draw_entities() const;

    };

}


#endif // ENTITY_MANAGER_H
