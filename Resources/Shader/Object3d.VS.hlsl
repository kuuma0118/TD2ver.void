#include "Object3d.hlsli"

struct WorldTransform
{
    float32_t4x4 world;
};

struct ViewProjection
{
    float32_t4x4 view;
    float32_t4x4 projection;
};

ConstantBuffer<WorldTransform> gWorldTransform : register(b0);
ConstantBuffer<ViewProjection> gViewProjection : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, mul(gWorldTransform.world, mul(gViewProjection.view, gViewProjection.projection)));
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gWorldTransform.world));
    //線形深度
    float z = (output.position.z - 0.1f) / (100.0f - 0.1f);
    output.depth = float32_t4(z, 0, 0, 0);

    return output;
}