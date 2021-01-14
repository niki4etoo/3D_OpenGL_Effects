#version 330 core
out vec4 FragColor;

//Material with texture properties
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	
	//light properties
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	// attenuation parameters
	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform float time;

void main()
{
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
	
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	ambient *= attenuation;
	
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
	diffuse *= attenuation;
	
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	specular *= attenuation;
	vec3 emission = vec3(0.0f);
	if(texture(material.specular, TexCoords).r == 0)
	{
		// apply emission texture
		emission = texture(material.emission, TexCoords).rgb;
		
		//some extra fun stuff
		emission = texture(material.emission, TexCoords + vec2(0.0, time)).rgb; //moving
		//emission = emission * sin(time * 0.5 + 0.5) * 2.0;						//fading
	}
	vec3 emissionMask = step(vec3(1.0f), vec3(1.0f) - specular);
	emission = emission * emissionMask;
	emission *= attenuation;
	
	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
} 

