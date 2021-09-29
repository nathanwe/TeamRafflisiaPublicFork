#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}



#shader fragment
#version 330 core

uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
}