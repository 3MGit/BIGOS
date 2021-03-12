// VertexShader code
struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
};

struct Light
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

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

cbuffer cbPerFrame: register(b0)
{
    float3 u_CameraPosition;
    Light u_Light;
};

cbuffer cbPerObject: register(b1)
{
    column_major float4x4 u_Transform;
    column_major float4x4 u_ViewProj;
    Material u_Material;
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
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

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
    ambient = u_Light.Ambient * u_Material.Ambient;

    // Diffuse
    float3 norm = normalize(input.normal);
    float3 lightDir = normalize(-u_Light.Direction);
    float diffuseFactor = dot(norm, lightDir);

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        // Specular
        float3 viewDir = normalize(u_CameraPosition - input.position.xyz);

        float3 reflectDir = reflect(-lightDir, norm);
        float specFactor = pow(max(dot(reflectDir, viewDir), 0.0), u_Material.Specular.w);

        specular = u_Light.Specular * (specFactor * u_Material.Specular);
        diffuse = u_Light.Diffuse * (diffuseFactor * u_Material.Diffuse);
    }

    float4 litColor = ambient + diffuse + specular;
    litColor.a = u_Material.Ambient.a;

    return litColor;
    //return float4(input.normal, 1.0f);
 }
