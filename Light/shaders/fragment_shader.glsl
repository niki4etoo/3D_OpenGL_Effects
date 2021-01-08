#version 420 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 Normal;

uniform vec3 lightPos;

in vec2 TexCoords1;
uniform sampler2D texture1;

in vec3 FragPos;

vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

void main()
{
	//Lighting
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 result = (ambient + diffuse) * objectColor;	
	FragColor = vec4(result, 1.0f);
	
	FragColor = texture(texture1, TexCoords1);
}
