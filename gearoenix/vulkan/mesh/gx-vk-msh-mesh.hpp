#ifndef GEAROENIX_VULKAN_MESH_HPP
#define GEAROENIX_VULKAN_MESH_HPP
#include "../../render/gx-rnd-build-configuration.hpp"
#ifdef GX_RENDER_VULKAN_ENABLED
#include "../../core/ecs/gx-cr-ecs-component.hpp"
#include "../../math/gx-math-vertex.hpp"
#include "../../render/mesh/gx-rnd-msh-mesh.hpp"
#include "../buffer/gx-vk-buf-buffer.hpp"

namespace gearoenix::vulkan::engine {
struct Engine;
}

namespace gearoenix::vulkan::mesh {
struct Mesh final : public render::mesh::Mesh, public core::ecs::Component {
    GX_GET_CREF_PRV(buffer::Buffer, vertex)
    GX_GET_CREF_PRV(buffer::Buffer, index)

private:
    Mesh(engine::Engine&, std::vector<math::BasicVertex> vertices, std::vector<std::uint32_t> indices) noexcept;

public:
    Mesh(Mesh&&) noexcept;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    [[nodiscard]] static std::shared_ptr<Mesh> construct(
        engine::Engine&, std::vector<math::BasicVertex> vertices, std::vector<std::uint32_t> indices) noexcept;
    ~Mesh() noexcept final;
    void set_component(const std::shared_ptr<core::ecs::EntitySharedBuilder>&) noexcept final;
    [[nodiscard]] std::pair<VkDeviceAddress, VkDeviceAddress> get_buffers_address() const noexcept;
};
}

#endif
#endif