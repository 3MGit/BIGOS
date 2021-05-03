struct VS_INPUT
{
	float3 positionL : POSITION;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float2 uv: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 positionCS : SV_POSITION;
	float4 positionL : POSITION;
};

cbuffer invBuffer : register(b4)
{
    float4x4 u_InvertViewProj;
	float4 u_Roughness;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 pos = float4(input.positionL, 1.0f);
    pos.z = 1.0f;
    output.positionCS = pos;
    output.positionL = mul(pos, u_InvertViewProj);
    return output;
}

struct PS_INPUT
{
	float4 positionCS : SV_POSITION;
	float4 positionL : POSITION;
};

static const float PI = 3.14159265359;

TextureCube u_EnvironmentMap : register(t5);
SamplerState u_CubemapSampler : register(s5);

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // From spherical coordinates to cartesian coordinates
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // From tangent-space floattor to world-space sample floattor
    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);

    float3 samplefloat = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(samplefloat);
}

float4 psmain(PS_INPUT input) : SV_Target
{
    float3 N = normalize(input.positionL.xyz);
    float3 R = N;
    float3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    float3 prefilteredColor = 0.0;
    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, N, u_Roughness.x);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            float3 modified = float3(L.x, -L.y, L.z);
            prefilteredColor += u_EnvironmentMap.Sample(u_CubemapSampler, modified).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    //return float4(0.0f, 1.0f, 0.0f, 1.0f);
    return float4(prefilteredColor, 1.0);
}