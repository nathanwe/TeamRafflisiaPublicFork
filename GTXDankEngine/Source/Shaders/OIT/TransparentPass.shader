#shader vertex
#version 460 core

// shader inputs
layout(location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
}




#shader fragment
#version 460 core

// shader outputs
// MRT
layout(location = 0) out vec4 accum;
layout(location = 1) out float reveal;

in vec2 TexCoord;

// material color
uniform sampler2D diffuse;
uniform float alpha;

// No lighting calculation, only diffuse color
void main()
{
	vec3 color = texture(diffuse, TexCoord).rgb;

	// weight function
	// https://jcgt.org/published/0002/02/09/
	// Page 129, Equation (10)
	float weight = clamp(pow(min(1.0, alpha * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);

	// store pixel color accumulation
	accum = vec4(color * alpha, alpha) * weight;

	// store pixel revealage threshold
	reveal = alpha;
}