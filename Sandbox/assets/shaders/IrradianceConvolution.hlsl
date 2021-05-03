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

struct PS_INPUT
{
	float4 positionCS : SV_POSITION;
	float4 positionL : POSITION;
};

static const float PI = 3.14159265359;

TextureCube u_EnvironmentMap : register(t5);
SamplerState u_CubemapSampler : register(s5);

float4 psmain(PS_INPUT input) : SV_Target
{
	float3 N = normalize(input.positionL.xyz);
	float3 irradiance = 0.0;
	
	// Tangent space calculation from origin point
	float3 up = float3(0.0, 1.0, 0.0);
	float3 right = normalize(cross(up, N));
	up = normalize(cross(N, right));

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// Spherical to cartesian (in tangent space)
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// Tangent space to world
			float3 samplefloat = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
			irradiance += u_EnvironmentMap.Sample(u_CubemapSampler, samplefloat).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	//float3 envColor = EnvironmentMap.Sample(CubemapSampler, input.positionL.xyz).rgb;
	return float4(irradiance, 1.0);
	//return float4(envColor, 1.0);
	//return float4(0.0f, 1.0f, 0.0f, 1.0f);
}