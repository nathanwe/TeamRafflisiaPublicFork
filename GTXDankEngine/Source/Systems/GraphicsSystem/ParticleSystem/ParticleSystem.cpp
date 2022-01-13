#include "pch.h"
#include "ParticleSystem.h"

#include "../utils/Log.h"


void ParticleSystem::Init(int num_particles)
{
	ComputeShader = new Shader("Source/Shaders/ParticleSystem/ParticleSystem.cs.shader");
	RenderShader = new Shader("Source/Shaders/ParticleSystem/Render.shader");



	for (int i = 0; i < num_particles; ++i)
		Particles.push_back(Particle());



	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_ARRAY_BUFFER, SSBO);
	glBufferData(GL_ARRAY_BUFFER, Particles.size() * sizeof(Particle), &Particles[0], GL_STATIC_DRAW);

	// render shader uniforms
	RenderShader->setInt("vertex_count", static_cast<int>(Particles.size()));
	RenderShader->setMat4("projection", glm::ortho(0.0f, (float)1200, (float)860, 0.0f, -1.0f, 1.0f));

}



void ParticleSystem::Draw(float timeStamp, glm::mat4 view, glm::mat4 proj, unsigned int FBO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Invoke Compute Shader and wait for all memory access to SSBO to safely finish

	ComputeShader->setFloat("timeStamp", timeStamp);

	ComputeShader->Bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glDispatchCompute(static_cast<GLuint>((Particles.size() / 128) + 1), 1, 1);	// group size: 128
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	ComputeShader->unBind();

	// Render the results
	RenderShader->setMat4("projection", proj);
	RenderShader->setMat4("view", view);

	RenderShader->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, SSBO);
	glDrawArraysInstanced(GL_POINTS, 0, 1, static_cast<GLsizei>(Particles.size()));
	RenderShader->unBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void ParticleSystem::Destory()
{
	glDeleteBuffers(1, &SSBO);
	glDeleteVertexArrays(1, &VAO);
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

	ss << "Number of Particles: " << Particles.size() << std::endl;

	LOG_INFO(ss.str());
}



float gen_random(float min, float max) {
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}