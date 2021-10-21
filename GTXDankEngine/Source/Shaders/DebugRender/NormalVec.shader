#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// used by geometry shader
out VS_OUT{
    vec3 normal;
} vs_out;

void main()
{
    // only model view transformation
    gl_Position = view * model * vec4(aPos, 1.0f);

    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}


#shader geometry
#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;


in VS_OUT{
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 projection;

// The line emit 2 vertices
void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = projection * (gl_in[index].gl_Position +
        vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    // since the input of geometry shader is a triangle which has 3 vertices
    // each vertex should generate a normal
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}



#shader fragment
#version 330 core

uniform vec3 segmentColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(segmentColor, 1.0f);
}