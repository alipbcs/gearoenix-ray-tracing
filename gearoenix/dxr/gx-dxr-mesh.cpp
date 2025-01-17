#include "gx-dxr-mesh.hpp"
#ifdef GX_RENDER_DXR_ENABLED
#include "../core/gx-cr-string.hpp"
#include "../render/material/gx-rnd-mat-pbr.hpp"
#include "../render/mesh/gx-rnd-msh-mesh.hpp"
#include "gx-dxr-buffer.hpp"
#include "gx-dxr-device.hpp"
#include "gx-dxr-engine.hpp"
#include "gx-dxr-texture.hpp"
#include "gx-dxr-uploader.hpp"
#include "shaders/gx-dxr-shd-common.hpp"
#include <map>

gearoenix::dxr::Mesh::Mesh(
    std::shared_ptr<GpuBuffer>&& in_vb,
    std::shared_ptr<GpuBuffer>&& in_ib,
    math::Aabb3<double>&& box,
    const UINT vertex_size,
    const UINT vertices_size,
    const UINT indices_count) noexcept
    : render::mesh::Mesh(std::move(box))
    , vb(std::move(in_vb))
    , ib(std::move(in_ib))
    , indices_count(indices_count)
    , vv {
        .BufferLocation = vb->get_resource()->GetGPUVirtualAddress(), // vertices
        .SizeInBytes = vertices_size,
        .StrideInBytes = vertex_size
    }
    , iv {
        .BufferLocation = ib->get_resource()->GetGPUVirtualAddress(), // indices
        .SizeInBytes = indices_count * 4,
        .Format = DXGI_FORMAT_R32_UINT // We're always going to use this
    }
{
}

gearoenix::dxr::Mesh::~Mesh() noexcept = default;

gearoenix::dxr::MeshManager::MeshManager(Engine& e) noexcept
    : render::mesh::Manager(e)
{
}

gearoenix::dxr::MeshManager::~MeshManager() noexcept = default;

std::shared_ptr<gearoenix::render::mesh::Mesh> gearoenix::dxr::MeshManager::build(
    std::string&& name,
    std::vector<render::PbrVertex>&& vertices,
    std::vector<std::uint32_t>&& indices,
    math::Aabb3<double>&& occlusion_box,
    core::sync::EndCallerIgnored&& c) noexcept
{
    auto& eng = dynamic_cast<Engine&>(e);
    auto* const d = eng.get_device()->get_device().Get();
    const auto& u = eng.get_uploader();

    const auto vbn = core::String::to_wstring(name + "-DXR-Vertex-Buffer");
    const auto vsz = vertices.size() * sizeof(render::PbrVertex);
    auto vb = std::make_shared<GpuBuffer>(d, static_cast<UINT>(vsz), vbn.c_str());
    std::vector<std::uint8_t> vbd(vsz);
    std::memcpy(vbd.data(), vertices.data(), vsz);

    const auto ibn = core::String::to_wstring(name + "-DXR-Index-Buffer");
    const auto isz = indices.size() * sizeof(std::uint32_t);
    auto ib = std::make_shared<GpuBuffer>(d, static_cast<UINT>(isz), ibn.c_str());
    std::vector<std::uint8_t> ibd(isz);
    std::memcpy(ibd.data(), indices.data(), isz);

    auto m = std::make_shared<Mesh>(
        std::shared_ptr<GpuBuffer>(vb),
        std::shared_ptr<GpuBuffer>(ib),
        std::move(occlusion_box),
        static_cast<UINT>(sizeof(render::PbrVertex)),
        static_cast<UINT>(vsz),
        static_cast<UINT>(indices.size()));

    core::sync::EndCallerIgnored end([c, m]() noexcept -> void {
        (void)c;
        (void)m;
    });

    u->upload(std::move(vbd), std::move(vb), core::sync::EndCallerIgnored(end));
    u->upload(std::move(ibd), std::move(ib), std::move(end));

    return m;
}

#endif
