#shader vertex
#version 330 core

layout(Location = 0) in vec3 aPos;
layout(Location = 1) in vec3 aNormal;
layout(Location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

void main()
{
	//gl_Position = projection * view * vec4(aPos, 1.0f);

	//vs_out.FragPos = aPos;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.TexCoord = aTexCoord;
}





#shader fragment
#version 330 core


in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D diffuse0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 FragColor;

vec4 pointLight() {
	vec3 lightVec = lightPos - fs_in.FragPos;
	float dist = length(lightVec);
	float a = 1.0;
	float b = 0.04;
	float inten = 1.0 / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.2f;


	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDirection = normalize(lightVec);

	float diffuse = max(dot(norm, lightDirection), 0.0f);

	float specLight = 0.5f;
	vec3 viewDirection = normalize(camPos - fs_in.FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(diffuse0, fs_in.TexCoord) * (diffuse * inten + ambient)) + specular * inten * lightColor;
}

void main()
{
	//vec3 color = vec3(1.0f);
	//vec3 normal = normalize(fs_in.Normal);

	//vec3 ambient = 0.05 * color;
	//vec3 diffuse = vec3(0.0f);

	//for (int i = 0; i < 4; ++i)
	//{
		//diffuse += calculateLighting(normal, i, color);
	//}


	//FragColor = vec4((ambient + diffuse) , 1.0f);   // testing, no shadow

	FragColor = pointLight();
}