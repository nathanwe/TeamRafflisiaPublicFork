#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(model) * aNormal;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}




#shader fragment
#version 330 core
layout(location = 0) out vec4 gAlbedoMetallic;
layout(location = 1) out vec4 gPosition;
layout(location = 2) out vec4 gNormalRoughness;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

struct Material {
    sampler2D texture_albedo;
    sampler2D texture_metallic;
};

uniform Material material;


void main()
{
    // store albedo vector into G buffer
    gAlbedoMetallic.rgb = texture(material.texture_albedo, TexCoords).rgb;
    // store metallic into G buffer
    gAlbedoMetallic.a = texture(material.texture_metallic, TexCoords).r;

    // store fragment position vector in the G buffer
    gPosition.rgb = WorldPos;

    gNormalRoughness.rgb = normalize(Normal);
}