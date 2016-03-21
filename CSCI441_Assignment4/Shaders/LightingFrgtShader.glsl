#version 430 core

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec4 vertexColor;
in vec4 fragPos;
in vec4 normal;

out vec4 color;

void main(void)
{
	const float ambientStrength = 0.1;
	const float specularStrength = 0.5;
	const vec4 lghtColor = vec4(1.0, 1.0, 1.0, 1.0);	// white light

	vec4 norm = normalize(normal);
	vec4 lightDir = normalize(vec4(lightPos, 1.0) - fragPos);
	vec4 viewDir = normalize(vec4(viewPos, 1.0) - fragPos);
	vec4 reflectDir = reflect(-lightDir, norm);
	float diff = max(dot(norm, lightDir), 0.0);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	// calc the ambient light
	vec4 ambientLght = ambientStrength*lghtColor;
	// calc the diffuse light
	vec4 diffuseLght = diff*lghtColor;
	// calc the specular light
	vec4 specularLght = specularStrength*spec*lghtColor;

	color = (ambientLght+diffuseLght+specularLght)*vertexColor;
}