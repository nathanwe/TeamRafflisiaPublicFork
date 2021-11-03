#shader vertex
#version 450

struct Particle
{
	vec3 position;
	vec3 velocity;
	float currTime;
	float timeLimit;
};

layout(std430, binding = 0) buffer ParticleData_t
{
	Particle particles[];
}
ParticleData;


//out float vertex_scale;
out float vertex_speed;
out int timeOut;

uniform float vertex_count;

void main() {
	//float depth_bias = ((2.0 / vertex_count) * gl_VertexID) - 1.0;
	//gl_Position = vec4(position, depth_bias, 1.0);

	Particle particle = ParticleData.particles[gl_InstanceID];

	gl_Position = vec4(particle.position, 1.0);

	//vertex_scale = particle.scale;
	vertex_speed = length(particle.velocity);

	// opengl does not support boolean
	if (particle.currTime > particle.timeLimit) timeOut = 1;
	else timeOut = 0;
};




#shader geometry
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

in float vertex_speed[];
in int timeOut[];


void main() {

	vec4 position = view * gl_in[0].gl_Position;
	const float speed = vertex_speed[0];
	const float scale =  0.5;

	float size = 0.008 * scale;

		gl_Position = projection * (position + vec4(-size, -size, 0.0, 0.0));
		EmitVertex();

		gl_Position = projection * (position + vec4(-size, size, 0.0, 0.0));
		EmitVertex();

		gl_Position = projection * (position + vec4(size, -size, 0.0, 0.0));
		EmitVertex();

		gl_Position = projection * (position + vec4(size, size, 0.0, 0.0));
		EmitVertex();

		EndPrimitive();
	//}
}



#shader fragment
#version 450


out vec4 FragColor;

void main() {
	
	vec4 color = vec4(0.6, 0.3, 0.0, 1.0);

	FragColor = color;
};