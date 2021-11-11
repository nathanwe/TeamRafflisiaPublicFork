#shader vertex
#version 330 core

uniform mat4 Proj;
uniform mat4 View;


out vec3 PS_IN_TexCoord;


void main(void)
{
    const vec3 vertices[4] = vec3[4](vec3(-1.0f, -1.0f, 1.0f),
        vec3(1.0f, -1.0f, 1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f));

	vec4 clip_pos = vec4(vertices[gl_VertexID].xy, -1.0, 1.0);
    vec3 dir = transpose(mat3(View)) * (inverse(Proj) * clip_pos).xyz; //vec3(inv_view * vec4(view_pos.x, view_pos.y, -1.0, 0.0));

    PS_IN_TexCoord = normalize(dir);

    gl_Position = vec4(vertices[gl_VertexID], 1.0f);
}







#shader fragment
#version 330 core


out vec4 PS_OUT_Color;

in vec3 PS_IN_TexCoord;

uniform vec3 u_Direction;


uniform vec3 A, B, C, D, E, F, G, H, I, Z;


vec3 hosek_wilkie(float cos_theta, float gamma, float cos_gamma)
{
	vec3 chi = (1 + cos_gamma * cos_gamma) / pow(1 + H * H - 2 * cos_gamma * H, vec3(1.5));
	return (1 + A * exp(B / (cos_theta + 0.01))) * (C + D * exp(E * gamma) + F * (cos_gamma * cos_gamma) + G * chi + I * sqrt(cos_theta));
}

// ------------------------------------------------------------------

vec3 hosek_wilkie_sky_rgb(vec3 v, vec3 sun_dir)
{
	float cos_theta = clamp(v.y, 0, 1);
	float cos_gamma = clamp(dot(v, sun_dir), 0, 1);
	float gamma_ = acos(cos_gamma);

	vec3 R = Z * hosek_wilkie(cos_theta, gamma_, cos_gamma);
	return R;
}



void main()
{
	vec3 dir = normalize(PS_IN_TexCoord);

	// dir: view direction
	// u_Direction: sun dir
	vec3 col = hosek_wilkie_sky_rgb(dir, u_Direction);

	PS_OUT_Color = vec4(col, 1.0);
}