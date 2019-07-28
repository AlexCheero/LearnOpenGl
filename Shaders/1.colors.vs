#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main()
{
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	Position = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * vec4(FragPos, 1.0);
	Normal = normalMatrix * aNormal;
	TexCoords = aTexCoords;
}