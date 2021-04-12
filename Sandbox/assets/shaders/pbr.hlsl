struct Material
{
    float4 Albedo;
    float usingAlbedoMap;
    float Metalic;
    float usingMetalicMap;
    float Roughness;
    float usingRoughnessMap;
    float AO;
};

struct Light
{
    float4 Color;
    float3 Position;
    float3 Direction;
    float  Intensity;
};

float PI = 3.14159265359;

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
    output.position = mul(float4(input.position, 1.0f), u_Transform);
    output.positionH = mul(output.position, u_ViewProj);
    output.normal = mul(input.normal, wsTransform);
    output.tangent = mul(input.tangent, wsTransform);
    output.uv = input.uv;

    return output;
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    //float NdotH = saturate(dot(N, H));
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
    //float NdotV = saturate(dot(N, V));
    //float NdotL = saturate(dot(N, L));
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

Texture2D u_Texture : register(t0);
SamplerState u_TextureSampler : register(s0);

float4 psmain(PS_INPUT input) : SV_Target
{
    float3 N = normalize(input.normal);
    float3 V = normalize(u_CameraPosition - input.position.xyz);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, u_Material.Albedo.rbg, u_Material.Metalic);

    // reflectance equation
    float3 Lo = float3(0.0, 0.0, 0.0);

    // calculate per-light radiance
    float3 L = normalize(u_Light.Position - input.position.xyz);
    float3 H = normalize(V + L);
    float distance = length(u_Light.Position - input.position.xyz);
    float attenuation = 1.0 / (distance * distance);
    float3 radiance = u_Light.Color.rbg * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, u_Material.Roughness);
    float G = GeometrySmith(N, V, L, u_Material.Roughness);
    float3 F = fresnelSchlick(saturate(dot(H, V)), F0);

    float3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    //float denominator = 4 * saturate(dot(N, V)) * saturate(dot(N, L));
    float3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

    // kS is equal to Fresnel
    float3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - u_Material.Metalic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);
    //float NdotL = saturate(dot(N, L));

    // add to outgoing radiance Lo
    Lo += (kD * u_Material.Albedo.rbg / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again


    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    float3 ambient = float3(0.03, 0.03, 0.03) * u_Material.Albedo.rbg * u_Material.AO;

    //float3 ambient = u_Material.Albedo.rbg * u_Material.AO;

    float3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + float3(1.0, 1.0, 1.0));
    // gamma correct
    color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

    return float4(color, 1.0f);
    //return float4(G, 0.0f, 0.0f, 1.0f);
}