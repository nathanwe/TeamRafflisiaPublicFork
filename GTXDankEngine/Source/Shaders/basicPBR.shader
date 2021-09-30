#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
}





#shader fragment
#version 330 core


in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;


const float PI = 3.141f;

// N_LIGHTS is the max value
const int N_LIGHTS = 200;	// don't inflate
uniform vec3 lightPositions[N_LIGHTS];
uniform vec3 lightColors[N_LIGHTS];


uniform sampler2D albedoTex;
uniform sampler2D metallicTex;
uniform sampler2D normalTex;
uniform sampler2D roughnessTex;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 FragColor;



// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------


vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalTex, fs_in.TexCoord).rgb * 2.0 - 1.0;

    vec3 Q1 = dFdx(fs_in.FragPos);
    vec3 Q2 = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoord);
    vec2 st2 = dFdy(fs_in.TexCoord);

    vec3 N = normalize(fs_in.Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// reflection equation
vec3 reflection(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, vec3 F0, vec3 WorldPos)
{
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < N_LIGHTS; ++i)
    {
        float d = length(lightPos - WorldPos);



        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = vec3(lightColors[i]) * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0, roughness);

        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    return Lo;
}



void main()
{
    vec3 WorldPos = fs_in.FragPos;

    vec3 albedo = texture(albedoTex, fs_in.TexCoord).rgb;

    float metallic = texture(metallicTex, fs_in.TexCoord).r;

    float roughness = texture(roughnessTex, fs_in.TexCoord).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N);    // reflection direction

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = mix(vec3(0.04f), albedo, metallic);

    // reflectance equation
    vec3 Lo = reflection(N, V, albedo, metallic, roughness, F0, WorldPos);

    vec3 ambient = albedo * 0.06;

    FragColor = vec4(Lo + ambient, 1.0f);
}