#version 330 core
// Position
layout (location = 0) in vec3 aPos;
// Normal
layout (location = 1) in vec3 aNormal;
// Color
layout (location = 2) in vec2 aTex;


out vec3 currentPos;
out vec3 normal;

out vec2 texCoord;




uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
	currentPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));

	normal = aNormal;
	//texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	texCoord =  aTex;

	gl_Position = camMatrix * vec4(currentPos, 1.0);

}