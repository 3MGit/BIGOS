// VertexShader code

struct VS_INPUT
{
    float3 position: POSITION;
    float4 color: COLOR;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float4 color: COLOR;
};

cbuffer cbPerObject: register(b0)
{
    column_major float4x4 u_World;
    column_major float4x4 u_ViewProj;
    float4 u_Color;
};

VS_OUTPUT vsmain( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //WORLD SPACE
    output.position = mul(float4(input.position, 1.0f), u_World);
    //VIEW PROJECTION SPACE
    output.position = mul(output.position, u_ViewProj);
    //output.color = input.color;
    output.color = u_Color;

    return output;
}

// PixelShader code

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float4 color: COLOR;
};

 float4 psmain( PS_INPUT input ) : SV_Target
 {
    return float4(input.color);
 }
