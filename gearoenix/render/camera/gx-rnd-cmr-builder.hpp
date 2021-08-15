#ifndef GEAROENIX_RENDER_CAMERA_BUILDER_HPP
#define GEAROENIX_RENDER_CAMERA_BUILDER_HPP
#include "../../core/ecs/gx-cr-ecs-entity.hpp"
#include "gx-rnd-cmr-projection.hpp"
#include <memory>

namespace gearoenix::render::engine {
struct Engine;
}

namespace gearoenix::render::camera {
struct Manager;
struct Builder final {
    friend struct Manager;

private:
    const std::shared_ptr<core::ecs::EntitySharedBuilder> entity_builder;

public:
    Builder(Builder&&) = delete;
    Builder(const Builder&) = delete;
    Builder& operator=(Builder&&) = delete;
    Builder& operator=(const Builder&) = delete;

    Builder(engine::Engine& e) noexcept;
    ~Builder() noexcept;

    void set_projection() noexcept;
};
// struct Uniform {
//     math::Vec3<float> position = math::Vec3(0.0f, 0.0f, 0.0f);
//     float aspect_ratio = 1.7f;
//     /// far is negative
//     float far = -100.0f;
//     /// near is negative
//     float near = -1.0f;
//     float clip_width = 0.0f;
//     float clip_height = 0.0f;
//     float clip_start_x = 0.0f;
//     float clip_start_y = 0.0f;
//     float hdr_tune_mapping = 1.0f;
//     float gamma_correction = 2.2f;
//     math::Mat4x4<float> inverted_rotation;
//     math::Mat4x4<float> view;
//     math::Mat4x4<float> projection;
//     math::Mat4x4<float> uniform_projection;
//     math::Mat4x4<float> view_projection;
//     math::Mat4x4<float> uniform_view_projection;
// };
}
#endif