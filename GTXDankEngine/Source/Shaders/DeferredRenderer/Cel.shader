#shader vertex
#version 330 core
layout(Location = 0) in vec2 aPos;
layout(Location = 1) in vec2 aTextureCoord;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    TexCoords = aTextureCoord;
}







#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float fraction;

uniform vec3 camPos;

uniform sampler2D gPosition;
uniform sampler2D gAlbedoMetallic;
uniform sampler2D gNormalRoughness;

const float PI = 3.141f;

const int MAX_LIGHTS = 200;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];

uniform vec3 directionalLightDirection;
uniform vec3 directionalLightColor;
uniform bool hasDirectionalLight;

uniform int numberOfLights;

uniform bool hasShadow;
uniform sampler2D ShadowMap;
uniform mat4 LightSpaceMatrix;
uniform bool EnablePCF;

// hard coded for specular
float shininess = 16;
float ambientFactor = 0.01;


//--------------------------------------------------------------------
// Basic shadowMap
float CalculateShadow(vec3 N, vec3 worldPos)
{
    vec4 lightSpaceFragPos = LightSpaceMatrix * vec4(worldPos, 1.0f);
    vec3 projCoord = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
    // transform to [0,1] range
    projCoord = projCoord * 0.5 + 0.5;

    if (projCoord.z > 1 || projCoord.z < 0) return 0;
    if (projCoord.x > 1 || projCoord.x < 0) return 0;
    if (projCoord.y > 1 || projCoord.y < 0) return 0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(ShadowMap, projCoord.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoord.z;
    //float currentDepth = lightSpaceFragPos.w;

    float bias = 0.0078f;
    // check whether current frag pos is in shadow
    //float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;


    float shadow = 0.0;

    if (EnablePCF)
    {
        vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(ShadowMap, projCoord.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
    }
    else
    {
        shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    }
    //if (closestDepth > 0.0f) return 1.0;
    return shadow;
}


vec3 LightLo(vec3 lightDir, vec3 lightColor, vec3 N, vec3 V, vec3 H, float spec, vec3 albedo)
{
    // calculate diffuse
    vec3 diffuse = max(dot(N, lightDir), 0.0) * lightColor;

    // calculate specular
    float specularFactor = max(dot(N, H), 0.0);
    vec3 specular = pow(specularFactor, shininess) * spec * lightColor;

    vec3 ambient = ambientFactor * lightColor;

    return (specular + diffuse + ambient) * albedo;
}

vec4 CelShading(vec3 N)
{
    float intensity;
    vec4 CelFactor;
    // base on angle between normal and light direction
    intensity = max(0.0, dot(normalize(directionalLightDirection), N));

    // 4 floors
    if (intensity > pow(0.95, fraction)) {
        CelFactor = vec4(vec3(1.0), 1.0);
    }
    else if (intensity > pow(0.5, fraction)) {
        CelFactor = vec4(vec3(0.6), 1.0);
    }
    else if (intensity > pow(0.25, fraction)) {
        CelFactor = vec4(vec3(0.4), 1.0);
    }
    else {
        CelFactor = vec4(vec3(0.2), 1.0);
    }
    return CelFactor;
}


void main()
{
    vec3 WorldPos = texture(gPosition, TexCoords).rgb;

    vec3 albedo = texture(gAlbedoMetallic, TexCoords).rgb;
    float spec = texture(gAlbedoMetallic, TexCoords).a;

    vec3 N = texture(gNormalRoughness, TexCoords).rgb;
    vec3 V = normalize(camPos - WorldPos);
    
    // Iterate all point light sources
    // calculate radiance
    vec3 Lo;
    for (int i = 0; i < numberOfLights; ++i)
    {
        float distance = length(lightPositions[i] - WorldPos);

        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);

        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        Lo += LightLo(L, radiance, N, V, H, spec, albedo);
    }

    // Directional light source
    if (hasDirectionalLight)
    {
        vec3 L = normalize(directionalLightDirection);
        vec3 H = normalize(V + L);

        float shadow = CalculateShadow(N, WorldPos);
        Lo += LightLo(L, directionalLightColor, N, V, H, spec, albedo) * (1.0 - shadow);
    }


    FragColor = CelShading(N) * vec4(Lo, 1.0);
}


