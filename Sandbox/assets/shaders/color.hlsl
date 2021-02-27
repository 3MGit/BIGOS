// VertexShader code
struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

struct Light
{
    float4 Color;
    float3 Position;
    float3 Direction;
};

struct VS_INPUT
{
    float3 position: POSITION;
    float3 normal: NORMAL;
};

struct VS_OUTPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal: NORMAL;
    float4 color: COLOR;
};

cbuffer cbPerObject: register(b0)
{
    column_major float4x4 u_Transform;
    column_major float4x4 u_ViewProj;
    float4 u_Color;
    float3 u_CameraPosition;
    Light u_Light;
};

VS_OUTPUT vsmain( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    //WORLD SPACE
    output.positionH = mul(float4(input.position, 1.0f), u_Transform);
    //VIEW PROJECTION SPACE
    output.positionH = mul(output.positionH, u_ViewProj);

    //PARAMETERS TO PS
    output.position = mul(float4(input.position, 1.0f), u_Transform);
    output.normal = mul(input.normal, (float3x3)u_Transform);
    output.color = u_Color;

    return output;
}

// PixelShader code
struct PS_INPUT
{
    float4 positionH: SV_POSITION;
    float4 position: POSITION;
    float3 normal : NORMAL;
    float4 color: COLOR;
};

float4 psmain(PS_INPUT input) : SV_Target
{
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Ambient
    float ambientStrength = 0.15;
    ambient = ambientStrength * u_Light.Color;

    // Diffuse
    float3 norm = normalize(input.normal);
    float3 lightDir = normalize(u_Light.Position - input.position.xyz);
    //float diff = max(dot(norm, lightDir), 0.0);
    float diff = dot(norm, lightDir);

    [flatten]
    if (diff > 0.0f)
    {
        // Specular
        float specularStrength = 0.5;
        float3 viewDir = normalize(u_CameraPosition - input.position.xyz);

        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        specular = specularStrength * spec * u_Light.Color;
        diffuse = diff * u_Light.Color;
    }
    return float4(ambient + diffuse + specular) * input.color;
    //return float4(input.normal, 1.0f);
 }
