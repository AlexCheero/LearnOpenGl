#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightPos;

void main()
{
	FragPos = vec3(modelView * vec4(aPos, 1.0));
	gl_Position = projection * vec4(FragPos, 1.0);
	Normal = normalMatrix * aNormal;
}