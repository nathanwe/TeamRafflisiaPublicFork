#shader vertex
#version 450

struct Particle
{
	vec3 position;
	vec3 velocity;
	float currTime;
	float timeLimit;
	
	bool alive;
	float randVelocity;
	float randAngle;

//	float randScale;
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


/// set alpha to zero if particle is dead
out float particle_alpha;
//out float particle_scale;

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

	if (particle.alive) particle_alpha = 1.0;
	else particle_alpha = 0.0;
};




#shader geometry
#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

in float vertex_speed[];
in int timeOut[];

/// pass through
in float particle_alpha[];
//in float particle_scale[];
out float n_particle_alpha;
out vec2 uvCoords;


void main() {

	vec4 position = view * gl_in[0].gl_Position;
	const float speed = vertex_speed[0];
//	const float scale =  0.5;
	const float scale =  10;

	float size = 0.04 * scale;

	
	for (int i = 0; i < 4; i++)
	{
		uvCoords.xy = vec2(1,1);
		float xMult = 1.0;
		if (floor(i/2) == 0)
		{
			xMult = -1;
			uvCoords.x = 0;
		}
		float yMult = 1.0;
		if (mod(i,2) == 0)
		{
			yMult = -1;
			uvCoords.y = 0;
		}
		gl_Position = projection * (position + vec4(size * xMult, size * yMult, 0.0, 0.0));
		n_particle_alpha = particle_alpha[i];
		EmitVertex();
	}

//		gl_Position = projection * (position + vec4(-size, -size, 0.0, 0.0));
//		n_particle_alpha = particle_alpha[0];
//		EmitVertex();

//		gl_Position = projection * (position + vec4(-size, size, 0.0, 0.0));
//		n_particle_alpha = particle_alpha[1];
//		EmitVertex();

//		gl_Position = projection * (position + vec4(size, -size, 0.0, 0.0));
//		n_particle_alpha = particle_alpha[2];
//		EmitVertex();

//		gl_Position = projection * (position + vec4(size, size, 0.0, 0.0));
//		n_particle_alpha = particle_alpha[3];
//		EmitVertex();

		EndPrimitive();
	//}
}



#shader fragment
#version 450

in float n_particle_alpha;
in vec2 uvCoords;

uniform bool haveTxtr;
uniform sampler2D txtr;


out vec4 FragColor;

void main() {

	vec4 color;
	
//	vec4 color = vec4(0.6, 0.3, 0.0, 1.0);
	if (haveTxtr)
	{
		color = texture(txtr, uvCoords);
	}
	else
	{
		color = vec4(0.6, 0.3, 0.0, n_particle_alpha);
	}

	FragColor = color;
};