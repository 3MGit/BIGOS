struct VS_INPUT
{
    float3 position: POSITION;
    float3 normal: NORMAL;
    float2 uv: TEXCOORD;
};

struct VS_OUTPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal: NORMAL;
    float2 uv: TEXCOORD;
};

Texture2D u_Texture : register(t0);
SamplerState u_TextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //PARAMETERS TO PS
    output.positionH = float4(input.position, 1.0f);
    output.position = float4(input.position, 1.0f);
    output.normal = input.normal;
    output.uv = input.uv;

    return output;
}

// PixelShader code
struct PS_INPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal : NORMAL;
    float2 uv: TEXCOORD;
};

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    texColor = u_Texture.Sample(u_TextureSampler, input.uv);

    float4 litColor = texColor;
  
    return litColor;   
}