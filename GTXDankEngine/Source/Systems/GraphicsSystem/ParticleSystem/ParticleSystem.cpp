#include "pch.h"
#include "ParticleSystem.h"

#include "../Core/Engine.h"

#include "../utils/Log.h"


extern Engine engine;

/// Particle functions
float particle_compare(Particle& p1, Particle& p2)
{
    return p1.timeLimit < p2.timeLimit;
}

float gen_random(float min, float max) {
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}
/// End of Particle functions


/// Particle Emitter methods
void ParticleEmitter::Init(unsigned int nParticleAmount, int peNum)
{
	particleEmitterNumber = peNum;
	
	/// copy particle shader files to separate them
	//std::ifstream inComputeShader   {"Source/Shaders/ParticleSystem/ParticleSystem.cs.shader"};
	//std::ifstream inRenderShader    {"Source/Shaders/ParticleSystem/Render.shader"};

	std::string cCSName("Source/Shaders/ParticleSystem/ParticleSystem"/*+std::to_string(fileNum)+*/".cs.shader");
	std::string cRSName("Source/Shaders/ParticleSystem/Render"/*+std::to_string(fileNum)+*/".shader");
	//std::ofstream copyComputeShader {cCSName};
	//std::ofstream copyRenderShader  {cRSName};

	/*std::string line;
	if (inComputeShader && copyComputeShader)
	{
		while (getline(inComputeShader, line))
		{
			copyComputeShader << line << "\n";
		}
	}
	if (inRenderShader && copyRenderShader)
	{
		while (getline(inRenderShader, line))
		{
			copyRenderShader << line << "\n";
		}
	}*/


    /// copied from ParticleSystem


	std::vector<Particle> Particles;
	for (int i = 0; i < nParticleAmount; ++i)
	{
		Particles.push_back(Particle(peNum));
	}
    maxActiveTime = std::max_element(Particles.begin(), Particles.end(), particle_compare)->timeLimit;

    particleAmount = nParticleAmount;
	ComputeShader = new Shader(cCSName);
	RenderShader = new Shader(cRSName);

	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_ARRAY_BUFFER, SSBO);
	glBufferData(GL_ARRAY_BUFFER, Particles.size() * sizeof(Particle), &Particles[0], GL_STATIC_DRAW);

	// render shader uniforms
	RenderShader->setInt("vertex_count", static_cast<int>(Particles.size()));
	RenderShader->setMat4("projection", glm::ortho(0.0f, (float)engine.GraphicsSys.camera.width, (float)engine.GraphicsSys.camera.height, 0.0f, -1.0f, 1.0f));
}


void ParticleEmitter::Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO)
{
	//LOG_INFO("Particle Emitter {0}: {1}", particleEmitterNumber, (busy || active ? "true" : "false"))
	//if (busy || active)
	//{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Invoke Compute Shader and wait for all memory access to SSBO to safely finish

		ComputeShader->setInt("cpeOwner", particleEmitterNumber);
		ComputeShader->setFloat("timeStamp", timeStamp);
		ComputeShader->setInt("emitterTest", 1);
		ComputeShader->setInt("disable", 0);
		ComputeShader->setInt("activate", 0);
		if (active)
		{
			ComputeShader->setVec3("sourcePosition", actPosition);
//			ComputeShader->setVec3("sourcePosition", glm::vec3(10, 10, 0));
			ComputeShader->setVec3("sourcePower", actForce);
			ComputeShader->setInt("activate", 1);
			ComputeShader->setFloat("anglePower", particleSpread);
			active = false;
			busy = true;
		}
		if (busy)
		{
			curActiveTime += timeStamp;
			if (curActiveTime > maxActiveTime)
			{
				ComputeShader->setInt("disable", 1);
				busy = false;
			}
		}

		ComputeShader->Bind();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
		glDispatchCompute(static_cast<GLuint>((particleAmount / 128) + 1), 1, 1);	// group size: 128
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
		ComputeShader->unBind();

		if (busy)
		{
			RenderShader->setInt("haveTxtr", haveTexture);
			if (haveTexture)
			{
				auto pTxtr = TextureResourceManger.GetResourceHandle(particleTexture);
				RenderShader->setTexture("txtr", pTxtr->GetPointer()->GetID());
			}
			RenderShader->setInt("haveColor", haveColor);
			if (haveColor)
			{
				RenderShader->setVec3("partColor", color);
			}

			// Render the results
			RenderShader->setMat4("projection", proj);
			RenderShader->setMat4("view", view);

			RenderShader->Bind();
			glBindBuffer(GL_ARRAY_BUFFER, SSBO);
			glDrawArraysInstanced(GL_POINTS, 0, 1, static_cast<GLsizei>(particleAmount));
			//	glDrawArraysInstanced(GL_TRIANGLES, 0, 1, static_cast<GLsizei>(particleAmount));
			RenderShader->unBind();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}
}


void ParticleEmitter::Destroy()
{
	glDeleteBuffers(1, &SSBO);
	glDeleteVertexArrays(1, &VAO);
}



void ParticleEmitter::Burst(glm::vec3& burstPosition, glm::vec3& burstForce, float spread, std::string texturePath)
{
    active = true;
    actPosition = burstPosition;
    actForce = burstForce;
	curActiveTime = 0.0f;
	particleSpread = spread;
	if (texturePath.empty())
		haveTexture = false;
	else
	{
		particleTexture = texturePath;
		haveTexture = true;
	}
}


void ParticleEmitter::Disable()
{
    ComputeShader->setInt("disable", 1);
    ComputeShader->setInt("activate", 0);
	ComputeShader->Bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glDispatchCompute(static_cast<GLuint>((particleAmount / 128) + 1), 1, 1);	// group size: 128
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	ComputeShader->unBind();
}
/// End of Particle Emitter methods




/// Particle System methods
void ParticleSystem::Init(int num_particles)
{
	/// currently we will have 10 particle emitters
	int particlesPerEmitter = num_particles / 10;
//	ParticleEmitters = std::vector<ParticleEmitter>(10);
//	for (auto& pe : ParticleEmitters)
//		pe.Init(particlesPerEmitter);

	for (int i = 0; i < 10; ++i)
	{
		ParticleEmitter nPE;
		nPE.Init(particlesPerEmitter, i);
		ParticleEmitters.push_back(nPE);
	}

	return;
}



void ParticleSystem::Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO)
{
	for (auto& pe : ParticleEmitters)
		pe.Draw(timeStamp, view, proj, FBO);


/*	elapsedTime += timeStamp;
	if (elapsedTime >= activateTime)
	{
		ActivateParticles(128,currActivPos,glm::vec3(0,1,0));
		currActivPos += glm::vec3(5,0,0);
		if (currActivPos.x - startActivPos.x > 25)
			currActivPos = startActivPos;
	}*/
}


void ParticleSystem::ActivateParticles(int numOfParticles, glm::vec3 nPosition, glm::vec3 nForce, float spread, std::string texturePath)
{
//	std::string& partPath = particlePaths[currPart++];
//	if (currPart >= particlePaths.size())
//		currPart = 0;
	elapsedTime = 0.0f;
	for (auto& pe : ParticleEmitters)
	{
		if (pe.active || pe.busy)
			continue;
		
		numOfParticles -= pe.particleAmount;
		if (texturePath.empty())
			pe.Burst(nPosition, nForce, spread);
		else
			pe.Burst(nPosition, nForce, spread, texturePath);
		if (numOfParticles <= (pe.particleAmount / 20))
			break;
	}
}


void ParticleSystem::Destroy()
{
	for (auto& pe : ParticleEmitters)
		pe.Destroy();

//	glDeleteBuffers(1, &SSBO);
//	glDeleteVertexArrays(1, &VAO);
}



void ParticleSystem::Print()
{
	std::stringstream ss;
	ss << "Compute Shader Capabilities:" << std::endl;
	ss << "GL_MAX_COMPUTE_WORK_GROUP_COUNT:" << std::endl;
	for (int i : {0, 1, 2}) {
		int tmp = 0;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, i, &tmp);
		ss << tmp << std::endl;
	}

	ss << "GL_MAX_COMPUTE_WORK_GROUP_SIZE:" << std::endl;
	for (int i : {0, 1, 2}) {
		int tmp = 0;
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &tmp);
		ss << tmp << std::endl;
	}

	ss << "Number of Particles: " << size << std::endl;

	LOG_INFO(ss.str());
}


void ParticleSystem::ResetEmitters()
{
	for (auto& emtr : ParticleEmitters)
	{
		emtr.active = false;
		emtr.busy = false;
		emtr.Disable();
	}
}
/// End of Particle System methods


