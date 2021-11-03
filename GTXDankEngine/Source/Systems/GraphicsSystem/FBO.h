#ifndef FBO_H
#define FBO_H

class FBO
{
private:
	unsigned int Fbo, DepthBuffer, ColorBuffer;


public:
	/*
	* Generate a FBO, buffer size = width * height
	* color buffer is a texture, depth buffer and stencil buffer are renderbuffer
	*/
	FBO() = default;

	void Init(unsigned int width, unsigned int height);

	inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, Fbo); };
	inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

	inline unsigned int GetFBO() const { return Fbo; }
	inline unsigned int GetDepth() { return DepthBuffer; };
	inline unsigned int GetColorAttachment() const { return ColorBuffer; };
};

#endif