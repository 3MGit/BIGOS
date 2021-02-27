// VertexShader code
struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

struct Light
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct VS_INPUT
{
    float3 position: POSITION;
    float3 normal: NORMAL;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float3 normal: NORMAL;
    float4 color: COLOR;
};

cbuffer cbPerObject: register(b0)
{
    column_major float4x4 u_World;
    column_major float4x4 u_ViewProj;
    float4 u_Color;
};

cbuffer PScbPerFrame: register(b1)
{
    Light u_Light;
};

VS_OUTPUT vsmain( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //WORLD SPACE
    output.position = mul(float4(input.position, 1.0f), u_World);
    //VIEW PROJECTION SPACE
    output.position = mul(output.position, u_ViewProj);
    output.color = u_Color;

    return output;
}

// PixelShader code
struct PS_INPUT
{
    float4 position: SV_POSITION;
    float3 normal : NORMAL;
    float4 color: COLOR;
};

 float4 psmain( PS_INPUT input ) : SV_Target
 {
    return float4(input.color);
 }
