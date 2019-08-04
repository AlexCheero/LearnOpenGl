#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

in vec3 vFragPos[];
in vec3 vNormal[];
in vec2 vTexCoords[];

out vec3 fFragPos;
out vec3 fNormal;
out vec2 fTexCoords;

void main()
{   
	fFragPos = vFragPos[0];
	fNormal = vNormal[0];
	fTexCoords = vTexCoords[0];

    gl_Position = gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}  