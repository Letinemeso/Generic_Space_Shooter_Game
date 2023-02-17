#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <Data_Structures/List.h>

#include <Renderer.h>
#include <Physics/Collision_Detector_2D.h>

#include <Entity.h>


namespace GSSG
{

    class Entity_Manager
    {
    private:
        LEti::Collision_Detector_2D* m_collision_detector = nullptr;
        const LEti::Renderer* m_renderer = nullptr;
        const LEti::Camera_2D* m_camera = nullptr;

    private:
        LDS::List<Entity*> m_entities;

    private:
        float m_max_distance_from_view_pos = 1000.0f;

    public:
        Entity_Manager();
        ~Entity_Manager();

    public:
        inline void inject_collision_detector(LEti::Collision_Detector_2D* _collision_detector) { m_collision_detector = _collision_detector; }
        inline void inject_renderer(LEti::Renderer* _renderer) { m_renderer = _renderer; }
        inline void inject_camera(LEti::Camera_2D* _camera) { m_camera = _camera; }

        inline void set_max_distance_from_view_pos(float _max) { m_max_distance_from_view_pos = _max; }

    private:
        void M_delete_entity(Entity* _entity);

    public:
        void add_entity(Entity* _entity);

    public:
        void update_entities_prev_state() const;
        void update_entities(float _ratio = 1.0f) const;

        void remove_dead_entities();

        void draw_entities() const;

    };

}


#endif // ENTITY_MANAGER_H
