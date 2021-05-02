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

cbuffer invBuffer : register(b0)
{
	float4x4 u_InvertViewProj;
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


Texture2D u_EnvironmentMap : register(t0);
SamplerState u_EnvironmentMapSampler : register(s0);

struct PS_INPUT
{
	float4 positionCS : SV_POSITION;
	float4 positionL : POSITION;
};

static const float2 invAtan = float2(0.1591, 0.3183);
float2 SampleSphericalMap(float3 v)
{
	float2 uv = float2(atan2(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

float4 psmain(PS_INPUT input) : SV_Target
{
	float2 texcoord = SampleSphericalMap(normalize(input.positionL.xyz));

	float4 color = u_EnvironmentMap.Sample(u_EnvironmentMapSampler, texcoord);

	return color;
	//return float4(0.0f, 1.0f, 0.0f, 1.0f);
}