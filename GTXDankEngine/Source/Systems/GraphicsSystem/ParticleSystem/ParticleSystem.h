#pragma once
#include "pch.h"
#include "../Core/Shader.h"
#include <vector>
#include <chrono>
#include <random>
// This is a 2D Particle system

using namespace std::chrono;


float gen_random(float min, float max);


struct Particle
{
	Particle()
		:position(gen_random(-1.0, 1.0), gen_random(-1.0, 1.0), gen_random(-1.0, 1.0)),
		velocity(0.0f, gen_random(0.0, 1), gen_random(0.0, 1)),
		currTime(0),
		timeLimit(gen_random(0, 5.0))
	{
		//std::cout << "Particle position: " << position.x << ", " << position.y << std::endl;
	}

	glm::vec3 position;
	glm::vec3 velocity;
	float currTime;
	float timeLimit;		
};


enum EmissionShape
{
	EMISSION_SHAPE_SPHERE,
	EMISSION_SHAPE_BOX,
	EMISSION_SHAPE_CONE
};

enum DirectionType
{
	DIRECTION_TYPE_SINGLE,
	DIRECTION_TYPE_OUTWARDS
};




class ParticleSystem
{

public:
	ParticleSystem() = default;

	void Init(int num_particles = 10);
	void Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO);
	void Destory();
	void Print();



private:
	unsigned int VAO;
	unsigned int SSBO;

	Shader* RenderShader;
	Shader* ComputeShader;

	std::vector<Particle> Particles;
};

