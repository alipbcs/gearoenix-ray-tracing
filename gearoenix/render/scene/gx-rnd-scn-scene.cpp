#include "gx-rnd-scn-scene.hpp"
#include "../../core/ecs/gx-cr-ecs-world.hpp"
#include "../../core/macro/gx-cr-mcr-assert.hpp"
#include "../camera/gx-rnd-cmr-camera.hpp"
#include "../engine/gx-rnd-eng-engine.hpp"
#include "../model/gx-rnd-mdl-model.hpp"
#include <algorithm>

gearoenix::render::scene::Scene::Scene(engine::Engine& e, const double layer) noexcept
    : core::ecs::Component(this)
    , e(e)
    , layer(layer)
{
}

gearoenix::render::scene::Scene::~Scene() noexcept = default;

gearoenix::render::scene::Scene::Scene(Scene&&) noexcept = default;

void gearoenix::render::scene::Scene::add_model(const core::ecs::Entity::id_t entity, model::Model&) noexcept
{
    model_entities.insert(entity);
    entities.insert(entity);
}

void gearoenix::render::scene::Scene::add_camera(const core::ecs::Entity::id_t entity, camera::Camera&) noexcept
{
    camera_entities.insert(entity);
    entities.insert(entity);
}

void gearoenix::render::scene::Scene::update(const core::ecs::Entity::id_t scene_entity_id) noexcept
{
    auto* const world = e.get_world();
    std::uint64_t flag = 1;
    cameras_flags.clear();
    world->synchronised_system<camera::Camera>([&](const core::ecs::Entity::id_t id, camera::Camera& cam) {
        if (!cam.get_is_enabled() || cam.get_scene_id() != scene_entity_id)
            return;
        cameras_flags[id] = flag;
        cam.set_flag(flag);
        flag <<= 1;
    });
    world->parallel_system<model::Model>([&](const core::ecs::Entity::id_t, model::Model& mdl, const unsigned int) {
        if (!mdl.enabled || mdl.scene_id != scene_entity_id)
            return;
        mdl.block_cameras_flags = static_cast<std::uint64_t>(-1);
        for (const auto blocked_camera : mdl.blocked_cameras)
            if (auto blocked_camera_search = cameras_flags.find(blocked_camera); cameras_flags.end() != blocked_camera_search)
                mdl.block_cameras_flags &= ~blocked_camera_search->second;
    });
}