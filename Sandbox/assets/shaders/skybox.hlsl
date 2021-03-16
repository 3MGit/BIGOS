struct VS_INPUT
{
	float3 positionL : POSITION;
	float3 normal: NORMAL;
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


TextureCube u_EnvironmentMap : register(t0);
SamplerState u_EnvironmentMapSampler : register(s0);

struct PS_INPUT
{
	float4 positionCS : SV_POSITION;
	float4 positionL : POSITION;
};

float4 psmain(PS_INPUT input) : SV_Target
{
	//return float4(u_EnvironmentMap.SampleLevel(u_EnvironmentMapSampler, input.positionL, 4));
	return float4(u_EnvironmentMap.Sample(u_EnvironmentMapSampler, input.positionL.xyz));
}