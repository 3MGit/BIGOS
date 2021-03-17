struct Material
{
    float4 u_Albedo;
    float u_usingAlbedoMap;
    float u_Metalic;
    float u_usingMetalicMap;
    float u_Roughness;
    float u_usingRoughnessMap;
    float u_AO;
};

struct Light
{
    float4 u_Color;
    float3 u_Position;
    float3 u_Direction;
    float  u_Intensity;
};



struct VS_INPUT
{
    float3 position: POSITION;
    float3 normal: NORMAL;
    float3 tangent : TANGENT;
    float2 uv: TEXCOORD;
};

struct VS_OUTPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal: NORMAL;
    float3 tangent : TANGENT;
    float2 uv: TEXCOORD;
};

struct PS_INPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv: TEXCOORD;
};

cbuffer cbPerFrame: register(b0)
{
    float3 u_CameraPosition;
    Light u_Light;
};

cbuffer cbPerObject: register(b1)
{
    column_major float4x4 u_Transform;
    column_major float4x4 u_ViewProj;
    column_major float4x4 u_InvModelViewProj;
};

cbuffer cbPerObjectMaterial: register(b2)
{
    Material u_Material;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    float3x3 wsTransform = (float3x3)u_Transform;

    VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(input.position, u_Transform);
    output.positionH = mul(output.position, u_ViewProj);
    output.normal = mul(input.normal, wsTransform);
    output.tangent = mul(input.tangent, wsTransform);
    output.uv = input.uv;

    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    
}