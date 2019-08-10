#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vFragPos[];
in vec3 vNormal[];
in vec2 vTexCoords[];

out vec3 fFragPos;
out vec3 fNormal;
out vec2 fTexCoords;

uniform float time;

vec3 GetNormal();
vec4 Explode(vec4 position, vec3 normal);

void main()
{   
	vec3 normal = GetNormal();
	fFragPos = vFragPos[0];
	fNormal = vNormal[0];
	fTexCoords = vTexCoords[0];
    gl_Position = /*Explode(gl_in[0].gl_Position, normal);//*/gl_in[0].gl_Position; 
    EmitVertex();

    fFragPos = vFragPos[1];
	fNormal = vNormal[1];
	fTexCoords = vTexCoords[1];
    gl_Position = /*Explode(gl_in[1].gl_Position, normal);//*/gl_in[1].gl_Position; 
    EmitVertex();

    fFragPos = vFragPos[2];
	fNormal = vNormal[2];
	fTexCoords = vTexCoords[2];
    gl_Position = /*Explode(gl_in[2].gl_Position, normal);//*/gl_in[2].gl_Position; 
    EmitVertex();

    EndPrimitive();
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 Explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0);
}