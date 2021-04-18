// VertexShader code
struct PhongMaterial
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
};

struct PhongLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Direction;
};

float3 NormalSampleToWorldSpace(float3 normalMapSample,
    float3 unitNormalW,
    float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;
    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    //float3 T = tangentW;
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);
    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);
    bumpedNormalW = normalize(bumpedNormalW);
    return bumpedNormalW;
}

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

cbuffer cbPerFrame: register(b0)
{
    float3 u_CameraPosition;
    PhongLight u_Light[4];
};

cbuffer cbPerObject: register(b1)
{
    column_major float4x4 u_Transform;
    column_major float4x4 u_ViewProj;
    column_major float4x4 u_InvModelViewProj;    
    PhongMaterial u_Material;
};

//Textures
//Texture2DMS : register(t0);
Texture2D u_Texture : register(t0);
SamplerState u_TextureSampler : register(s0);

Texture2D u_NormalMap : register(t1);
SamplerState u_NormalMapSampler : register(s1);

VS_OUTPUT vsmain( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //WORLD SPACE
    output.positionH = mul(float4(input.position, 1.0f), u_Transform);
    //VIEW PROJECTION SPACE
    output.positionH = mul(output.positionH, u_ViewProj);
    //PARAMETERS TO PS
    output.position = mul(float4(input.position, 1.0f), u_Transform);
    output.tangent = mul(input.tangent, (float3x3)u_Transform);
    output.normal = mul(input.normal, (float3x3)u_Transform);
    output.uv = input.uv;

    return output;
}

// PixelShader code
struct PS_INPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv: TEXCOORD;
};

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    //NormalMap
    float3 normalMapSample = u_NormalMap.Sample(u_NormalMapSampler, input.uv).rgb;

    // Ambient
    ambient = u_Light[0].Ambient * u_Material.Ambient;

    // Diffuse
    float3 norm = normalize(input.normal);
    //float3 norm = NormalSampleToWorldSpace(normalMapSample, input.normal, input.tangent);
    float3 lightDir = normalize(-u_Light[0].Direction);
    float diffuseFactor = dot(norm, lightDir);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        // Specular
        float3 viewDir = normalize(u_CameraPosition - input.position.xyz);

        float3 reflectDir = reflect(-lightDir, norm);
        float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), u_Material.Specular.w);

        specular = u_Light[0].Specular * (specFactor * u_Material.Specular);
        diffuse = u_Light[0].Diffuse * (diffuseFactor * u_Material.Diffuse);
    }

    texColor = u_Texture.Sample(u_TextureSampler, input.uv);

    float4 litColor = texColor * (ambient + diffuse) + specular;
    litColor.a = u_Material.Diffuse.a * texColor.a;

    return litColor;
    //return float4(input.normal, 1.0f);
 }
