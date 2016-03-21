#version 430 core

uniform mat4 rot;
uniform mat4 scale;
uniform mat4 viewMatrix;
uniform mat4 projection;

out vec4 vertexColor;
out vec4 fragPos;
out vec4 normal;

void main(void)
{
	// sides of the cube
	const vec4 vertices[24] = vec4[24](
	vec4(1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),

	vec4(1.0, -1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(1.0, -1.0, -1.0, 1.0),
 
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
	vec4(1.0, -1.0, 1.0, 1.0),
   
	vec4(1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(1.0, 1.0, -1.0, 1.0),
     
	vec4(-1.0, 1.0, 1.0, 1.0),
	vec4(-1.0, 1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, -1.0, 1.0),
	vec4(-1.0, -1.0, 1.0, 1.0),
   
	vec4(1.0, 1.0, -1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, -1.0, 1.0, 1.0),
	vec4(1.0, -1.0, -1.0, 1.0));
	
	// normals for the sides of the cube
	const vec4 faceNorms[6] = vec4[6](
	vec4(0.0, 1.0, 0.0, 0.0),	// top
	vec4(0.0, -1.0, 0.0, 0.0),	// bottom
	vec4(0.0, 0.0, 1.0, 0.0),	// right
	vec4(0.0, 0.0, -1.0, 0.0),	// left
	vec4(-1.0, 0.0, 0.0, 0.0),	// back
	vec4(1.0, 0.0, 0.0, 0.0));	// front
	
	// cube colors
	const vec4 cubeColor[6] = vec4[6](
	vec4(0.0, 1.0, 0.0, 1.0), 	// green
	vec4(1.0, 0.5, 0.0, 0.83), 	// orange
	vec4(1.0, 0.0, 0.0, 0.66),	// red
	vec4(1.0, 1.0, 0.0, 0.49),	// yellow
	vec4(0.0, 0.0, 1.0, 0.32),	// blue
	vec4(1.0, 0.0, 1.0, 0.15));	// magenta
	
	mat4 modelMatrix = rot*scale;
	
	gl_Position = projection*viewMatrix*modelMatrix*vertices[gl_VertexID];
	fragPos = modelMatrix*vertices[gl_VertexID];	// calc the fragments position
	vertexColor = cubeColor[gl_VertexID/4];
	normal = modelMatrix*faceNorms[gl_VertexID/4];
}