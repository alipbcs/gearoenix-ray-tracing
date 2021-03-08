#include "gx-vk-msh-accel-component.hpp"
#ifdef GX_RENDER_VULKAN_ENABLED
#include "gx-vk-msh-accel.hpp"

gearoenix::vulkan::mesh::AccelComponent::AccelComponent(std::shared_ptr<Accel> accel_mesh) noexcept
    : core::ecs::Component(this)
    , acceleration_structure(accel_mesh->get_vulkan_data())
    , acceleration_address(accel_mesh->get_acceleration_address())
    , acceleration_mesh(std::move(accel_mesh))
{
}

gearoenix::vulkan::mesh::AccelComponent::AccelComponent(AccelComponent&&) noexcept = default;

gearoenix::vulkan::mesh::AccelComponent::~AccelComponent() noexcept = default;

#endif