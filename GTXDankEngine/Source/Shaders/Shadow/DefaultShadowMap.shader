#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out float VertexDepth;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	VertexDepth = gl_Position.w;	// non distorted z value
}





#shader fragment
#version 330 core

in float VertexDepth;
out vec4 FragColor;


void main()
{
	// world space depth value in Red channel
	FragColor = vec4(VertexDepth, 0, 0, 0);
}