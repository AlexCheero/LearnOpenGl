#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	vec3 fragPos = vec3(modelView * vec4(aPos + aNormal * 0.2, 1.0));
	gl_Position = projection * vec4(fragPos, 1.0);
}