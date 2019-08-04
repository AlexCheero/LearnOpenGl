#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTexCoords;

/*
out vec3 fFragPos;
out vec3 fNormal;
out vec2 fTexCoords;
*/

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main()
{
	vFragPos = vec3(view * model * vec4(aPos, 1.0));
	vNormal = normalMatrix * aNormal;
	vTexCoords = aTexCoords;
	gl_Position = projection * vec4(vFragPos, 1.0);

	/*
	fFragPos = vec3(view * model * vec4(aPos, 1.0));
	fNormal = normalMatrix * aNormal;
	fTexCoords = aTexCoords;
	gl_Position = projection * vec4(fFragPos, 1.0);
	*/
}