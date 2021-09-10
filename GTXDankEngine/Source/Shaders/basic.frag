#version 330 core
out vec4 FragColor;

in vec3 color;

in vec2 texCoord;

in vec3 normal;
in vec3 currentPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// light radiates in all directions from a given source
vec4 pointLight() {
	vec3 lightVec = lightPos - currentPos;
	float dist = length(lightVec);
	float a = 1.0;
	float b = 0.04;
	float inten = 1.0 / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.2f;


	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightVec);

	float diffuse = max(dot(norm, lightDirection), 0.0f);

	float specLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

// light comes down from a large far away source, rays are basically parallel
vec4 directionLight() {
	// ambient lighting
	float ambient = 0.2f;


	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

	float diffuse = max(dot(norm, lightDirection), 0.0f);

	float specLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

// light that radiates from a single source, forming a cone
vec4 spotLight() {
	float outerCone = 0.90f;
	float innerCone = 0.95f;


	// ambient lighting
	float ambient = 0.2f;


	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float diffuse = max(dot(norm, lightDirection), 0.0f);

	float specLight = 0.5f;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main() {
	//FragColor = pointLight();
	FragColor = directionLight();
	//FragColor = spotLight();
}