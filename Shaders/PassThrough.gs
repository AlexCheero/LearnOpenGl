#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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

    fFragPos = vFragPos[1];
	fNormal = vNormal[1];
	fTexCoords = vTexCoords[1];
    gl_Position = gl_in[1].gl_Position; 
    EmitVertex();

    fFragPos = vFragPos[2];
	fNormal = vNormal[2];
	fTexCoords = vTexCoords[2];
    gl_Position = gl_in[2].gl_Position; 
    EmitVertex();

    EndPrimitive();
}  