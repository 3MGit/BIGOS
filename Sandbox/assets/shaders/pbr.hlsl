struct Material
{
    float4 Albedo;
    float usingAlbedoMap;
    float Metalic;
    float usingMetalicMap;
    float Roughness;
    float usingRoughnessMap;
    float AO;
    float usingAOMap;
    float usingNormalMap;
};

struct Light
{
    float4 Color;
    float3 Position;
    float3 Direction;
    //float  Intensity;
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

struct Attributes
{
    float4 position;
    float2 uv;
    float3 normal;
    float3 binormal;
    float3 tangent;
};


static const float PI = 3.14159265359f;
static const float GAMMA = 2.2f;

cbuffer cbPerFrame: register(b0)
{
    float3 u_CameraPosition;
    Light u_Lights[20];
    uint u_LightsCount;
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
    output.position = mul(float4(input.position, 1.0f), u_Transform);
    output.positionH = mul(output.position, u_ViewProj);
    output.normal = mul(input.normal, wsTransform);
    output.tangent = mul(input.tangent, wsTransform);
    output.uv = input.uv;

    return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// PBR ////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

Texture2D u_AlbedoTexture : register(t0);
Texture2D u_MetalicTexture : register(t1);
Texture2D u_RoughnessTexture : register(t2);
Texture2D u_AOTexture : register(t3);
Texture2D u_NormalTexture : register(t4);
SamplerState u_AlbedoSampler : register(s0);
SamplerState u_MetalicSampler : register(s1);
SamplerState u_RoughnessSampler : register(s2);
SamplerState u_AOSampler : register(s3);
SamplerState u_NormalSampler : register(s4);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// TEXTURES ///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


float4 GammaCorrectTexture(Texture2D t, SamplerState s, float2 uv)
{
    float4 samp = t.Sample(s, uv);
    return float4(pow(abs(samp.rgb), GAMMA), samp.a);
}

float4 GetAlbedo(Attributes a, Material m)
{
    return (1.0f - m.usingAlbedoMap) * m.Albedo + m.usingAlbedoMap * GammaCorrectTexture(u_AlbedoTexture, u_AlbedoSampler, a.uv);
}

float GetMetalic(Attributes a, Material m)
{
    return (1.0f - m.usingMetalicMap) * m.Metalic + m.usingMetalicMap * GammaCorrectTexture(u_MetalicTexture, u_MetalicSampler, a.uv).r;
}

float GetRoughness(Attributes a, Material m)
{
    return (1.0f - m.usingRoughnessMap) * m.Roughness + m.usingRoughnessMap * GammaCorrectTexture(u_RoughnessTexture, u_RoughnessSampler, a.uv).r;
}

float GetAO(Attributes a, Material m)
{
    return (1.0f - m.usingAOMap) * m.AO + m.usingAOMap * GammaCorrectTexture(u_AOTexture, u_AOSampler, a.uv).r;
}

float3 GetNormal(Attributes a, Material m)
{
    float3 N = a.normal;
    float3 T = a.tangent;
    float3 B = normalize(cross(N, T));

    float3x3 toWorld = float3x3(T, B, N);
    float3 normalMap = u_NormalTexture.Sample(u_NormalSampler, a.uv).rgb * 2.0 - 1.0;
    normalMap = mul(normalMap.rgb, toWorld);
    normalMap = normalize(normalMap);

    return (1.0f - m.usingNormalMap) * a.normal + m.usingNormalMap * normalMap;
}


float4 psmain(PS_INPUT input) : SV_Target
{
    Attributes attributes;
    attributes.position = input.position;
    attributes.uv = input.uv;
    attributes.normal = normalize(input.normal);
    attributes.tangent = normalize(input.tangent);

    float3 N = GetNormal(attributes, u_Material);
    float3 V = normalize(u_CameraPosition - attributes.position.xyz);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    float3 F0 = 0.04f;
    F0 = lerp(F0, GetAlbedo(attributes, u_Material).rgb, GetMetalic(attributes, u_Material));

    // reflectance equation
    float3 Lo = 0.0f;
    for (uint i = 0; i < 4; ++i)
    {

        // calculate per-light radiance
        float3 L = normalize(u_Lights[i].Position - attributes.position.xyz);
        float3 H = normalize(V + L);
        float distance = length(u_Lights[i].Position - attributes.position.xyz);
        float attenuation = 1.0f / (distance * distance);
        float3 radiance = u_Lights[i].Color.rbg * attenuation;
        //radiance = u_Light.Color.rbg;

        // Cook-Torrance BRDF
        //float NDF = DistributionGGX(N, H, u_Material.Roughness);
        //float G = GeometrySmith(N, V, L, u_Material.Roughness);
        float NDF = DistributionGGX(N, H, GetRoughness(attributes, u_Material));
        float G = GeometrySmith(N, V, L, GetRoughness(attributes, u_Material));
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0, GetRoughness(attributes, u_Material));

        float3 nominator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        float3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

        // kS is equal to Fresnel
        float3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        float3 kD = 1.0f - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - GetMetalic(attributes, u_Material);

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += 4.0f * ((kD * (GetAlbedo(attributes, u_Material).rgb / PI) + specular) * radiance * NdotL);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    float3 ambient = 0.03f * GetAlbedo(attributes, u_Material).rgb * GetAO(attributes, u_Material);

    //float3 ambient = u_Material.Albedo.rbg * u_Material.AO;

    float3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + 1.0f);
    // gamma correct
    color = pow(abs(color), 1.0f / 2.2f);

    return float4(color, 1.0f);
    //return float4(GetNormal(attributes, u_Material) *0.5f + 0.5f, 1.0f);
    //return GetAlbedo(attributes, u_Material);
}