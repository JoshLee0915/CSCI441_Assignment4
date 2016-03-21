#version 430 core

uniform mat4 scale;
uniform mat4 viewMatrix;
uniform mat4 projection;

out vec4 vertexColor;

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
	
	// cube colors
	const vec4 cubeColor[6] = vec4[6](
	vec4(0.0, 1.0, 0.0, 1.0), 	// green
	vec4(1.0, 0.5, 0.0, 1.0), 	// orange
	vec4(1.0, 0.0, 0.0, 1.0),	// red
	vec4(1.0, 1.0, 0.0, 1.0),	// yellow
	vec4(0.0, 0.0, 1.0, 1.0),	// blue
	vec4(1.0, 0.0, 1.0, 1.0));	// magenta
	
	mat4 modelMatrix = scale;
	
	gl_Position = projection*viewMatrix*modelMatrix*vertices[gl_VertexID];
	vertexColor = cubeColor[gl_VertexID/4];
}