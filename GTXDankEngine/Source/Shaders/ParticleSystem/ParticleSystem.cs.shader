#shader compute
#version 450 core

struct Particle {
	vec3 position;
	vec3 velocity;
	float currTime;
	float timeLimit;
	
	bool alive;
	float randVelocity;
	float randAngle;


	int peOwner;
};

layout(std430, binding = 0) buffer Particles {
	Particle particles[];
};

layout(local_size_x = 128) in;

uniform float timeStamp;


/// variables for testing emitter particle creation
uniform bool emitterTest;

uniform vec3 sourcePosition;
uniform vec3 sourcePower;
uniform bool activate;

uniform bool disable;

uniform float anglePower;

/// pass the emitter owner to be activated
uniform int cpeOwner;

/// static variable test
//uniform int statValue;

vec3 rotate(vec3 vect, float angle)
{
	return vec3(vect.x * cos(angle) - vect.y * sin(angle), vect.x * sin(angle) + vect.y * cos(angle), vect.z);
};

void main() {
	uint gid = gl_GlobalInvocationID.x;
	//statValue--;
	// if (!emitterTest)
	// {
	// 	if (!particles[gid].alive)
	// 	{
	// 		particles[gid].alive = true;
	// 	}

	// 	if (particles[gid].currTime > particles[gid].timeLimit);
		
	// 	particles[gid].currTime += timeStamp;

	// 	// Update position from velocity
	// 	particles[gid].position += particles[gid].velocity * timeStamp;

	// 	//particles[gid].statPartValue--;
	// }
	// else
	// {
	// 	/// only work with the particles that correlate to the particle emitter
	// 	//if (particles[gid].peOwner == cpeOwner)
	// 	//{
	// 		if (disable)
	// 		{
	// 			particles[gid].alive = false;
	// 		}
	// 		else if (activate && !particles[gid].alive)
	// 		{
	// 			particles[gid].alive = true;
	// 			particles[gid].currTime = 0.0;
	// 			particles[gid].position = sourcePosition;
	// 			particles[gid].velocity = sourcePower * particles[gid].randVelocity;
	// 			particles[gid].velocity = rotate(particles[gid].velocity, anglePower * particles[gid].randAngle);
	// //			particles[gid].velocity = rotate(particles[gid].velocity, particles[gid].randAngle);
	// 		}
	// 		else if (particles[gid].alive)
	// 		{
	// 			if (particles[gid].currTime > particles[gid].timeLimit)
	// 			{
	// 				particles[gid].alive = false;
	// 				particles[gid].currTime = 0.0;
	// 			}
			
	// 			particles[gid].currTime += timeStamp;

	// 			// Update position from velocity
	// 			particles[gid].position += particles[gid].velocity * timeStamp;
	// 		}
	// 	//}
	// }
	//if (particles[gid].peOwner == cpeOwner)
	//{
		if (disable)
		{
			particles[gid].alive = false;
		}
		else if (activate && !particles[gid].alive)
		{
			particles[gid].alive = true;
			particles[gid].currTime = 0.0;
			particles[gid].position = sourcePosition;
			particles[gid].velocity = sourcePower * particles[gid].randVelocity;
			particles[gid].velocity = rotate(particles[gid].velocity, anglePower * particles[gid].randAngle);
	//		particles[gid].velocity = rotate(particles[gid].velocity, particles[gid].randAngle);
		}
		else if (particles[gid].alive)
		{
			if (particles[gid].currTime > particles[gid].timeLimit)
			{
				particles[gid].alive = false;
				particles[gid].currTime = 0.0;
			}

			particles[gid].currTime += timeStamp;

			// Update position from velocity
			particles[gid].position += particles[gid].velocity * timeStamp;
		}
		//particles[gid].alive = true;
		//particles[gid].position = sourcePosition;
		//particles[gid].velocity = sourcePower;
	//}

	
//	if (particles[gid].peOwner == cpeOwner)
//	{
		//sparticles[gid].alive = true;
	//	particles[gid].position = sourcePosition;
	//	particles[gid].velocity = sourcePower;
//	}
}

