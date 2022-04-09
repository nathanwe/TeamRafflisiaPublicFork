#pragma once
#include "pch.h"
#include "../Core/Shader.h"
#include <vector>
#include <chrono>
#include <random>
// This is a 2D Particle system



#define M_PI 3.1415

float gen_random(float min, float max);


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


struct Particle
{
	Particle(int nPartEmitterOwner)
		:position(gen_random(0.0, 1.0), gen_random(0.0, 1.0), gen_random(0.0, 1.0)),
		velocity(0.0f, gen_random(0.0, 1), gen_random(0.0, 1)),
		currTime(0),
		timeLimit(gen_random(0, 0.5)),
		randVelocity(gen_random(0.5,2.5)),
		randAngle(gen_random(-M_PI / 4.0f, M_PI / 4.0f)),
		peOwner(nPartEmitterOwner)
	{
		//std::cout << "Particle position: " << position.x << ", " << position.y << std::endl;
	}

	Particle(const glm::vec3& startPos, const glm::vec3& direction)
		:position(startPos.x + gen_random(-1.0, 1.0), startPos.y + gen_random(-1.0, 1.0), startPos.z + gen_random(-1.0, 1.0)),
		velocity(gen_random(0.2, 1.5) * direction),
		currTime(0),
		timeLimit(gen_random(0, 5.0)),
		randVelocity(gen_random(0.5,25.0)),
		randAngle(gen_random(-M_PI / 20.0f, M_PI / 20.0f)),
		peOwner(-1)
	{
		//std::cout << "Particle position: " << position.x << ", " << position.y << std::endl;
	}

	glm::vec3 position;
	glm::vec3 velocity;
	float currTime;
	float timeLimit;


	bool alive = 0;
	float randVelocity;
	float randAngle;


	int peOwner;
};



class ParticleEmitter
{
/// methods
public:
    ParticleEmitter() = default;
    //~ParticleEmitter();

    //void Update(float deltaTime);

    /// used for simple burst effect and for constant emition
    void Burst(glm::vec3& burstPosition, glm::vec3& burstDirection, float spread, std::string texturePath = "");


	void Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO);
	void Destroy();
	void Init(unsigned int nParticleAmount, int peNum);


	void Disable();


/// parameters
public:

    /// should be used for permanent emitters
    bool active = false;
	bool busy = false;

	int particleAmount = 0;
	float particleSpread = 1.0f;
	
	int particleEmitterNumber;

/// methods
private:


/// parameters
private:
	/// personal shader parameters
	unsigned int VAO;
	unsigned int SSBO;

	Shader* RenderShader;
	Shader* ComputeShader;

    glm::vec3 actPosition;
    glm::vec3 actForce;

    /// emission parameters (shape and direction)
    EmissionShape shape;
    DirectionType dType;


	float maxActiveTime;
	float curActiveTime{0.0f};


    std::string particleTexture;
	bool haveTexture = false;
	
    glm::vec3 color;
	bool haveColor = false;


};




class ParticleSystem
{

public:
	ParticleSystem() = default;

	void Init(int num_particles = 10);
	void Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO);
	void Destroy();
	void Print();


	void ActivateParticles(int numOfParticles, glm::vec3 nPosition, glm::vec3 nForce, float spread, std::string texturePath = "");


	void ResetEmitters();


private:
	unsigned int VAO;
	unsigned int SSBO;

	Shader* RenderShader;
	Shader* ComputeShader;

	std::vector<ParticleEmitter> ParticleEmitters;

	int size = 0;

	const float activateTime = 1.5f;
	float elapsedTime = 2.0f;
	const glm::vec3 startActivPos{0.0, 0.0, -10.0};
	glm::vec3 currActivPos = startActivPos;

	int currPart = 0;
	std::vector<std::string> particlePaths{
		"Assets/Textures/CircleParticle.png",
		"Assets/Textures/StarParticle.png",
		"Assets/Textures/FireParticle.png",
		""
	};


};

