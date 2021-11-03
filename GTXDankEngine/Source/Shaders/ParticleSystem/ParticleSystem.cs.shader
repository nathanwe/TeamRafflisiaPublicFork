#shader compute
#version 450 core

struct Particle {
	vec3 position;
	vec3 velocity;
	float currTime;
	float timeLimit;
	
};

layout(std430, binding = 0) buffer Particles {
	Particle particles[];
};

layout(local_size_x = 128) in;

uniform float timeStamp;


void main() {
	uint gid = gl_GlobalInvocationID.x;

	if (particles[gid].currTime > particles[gid].timeLimit);
	
	particles[gid].currTime += timeStamp;

	// Update position from velocity
	particles[gid].position += particles[gid].velocity * timeStamp;
}