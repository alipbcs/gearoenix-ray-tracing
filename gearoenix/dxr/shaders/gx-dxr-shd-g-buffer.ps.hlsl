#include "gx-dxr-shd-g-buffer-common.hlsli"

[RootSignature(G_BUFFERS_FILLER_ROOT_SIGNATURE)]
float4 main(PSInput input) : SV_TARGET
{
    return tex2ds[ModelUniform.sampler_albedo_normal_emission.y].Sample(
        samplers[ModelUniform.sampler_albedo_normal_emission.x], input.uv) * ModelUniform.colour_factor;
}
