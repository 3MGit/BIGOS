struct Material
{

};

struct Light
{

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
    VS_OUTPUT output = (VS_OUTPUT)0;


    return output;
}

float4 psmain(PS_INPUT input) : SV_Target
{
    
}