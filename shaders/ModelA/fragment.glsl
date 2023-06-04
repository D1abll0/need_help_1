#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D mask;
	float shininess;
	vec3 color;
}; 

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;	
	
	float cutOff;
	float outerCutOff;
	
	vec3 type;		/*Direction Point Spot*/
};

#define NUM_OF_LIGHTS 3

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light[NUM_OF_LIGHTS];

void main()
{
	vec3 mask = vec3(texture(material.mask, TexCoords));
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result;

	for(int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		vec3 lightDir;
		/*universal lightDir*/
		lightDir = normalize(-light[i].direction * (light[i].type[0]) + (light[i].position - FragPos) * max(light[i].type[1], light[i].type[2]));  
		
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

		float distance = length(light[i].position - FragPos);
		float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));    

		float theta = dot(lightDir, normalize(-light[i].direction)); 
		float epsilon = light[i].cutOff - light[i].outerCutOff;
		float intensity = clamp((theta - light[i].outerCutOff) / epsilon, 0.0, 1.0);

		vec3 ambient = light[i].ambient * vec3(texture(material.diffuse, TexCoords));
		vec3 diffuse = light[i].diffuse * diff * vec3(texture(material.diffuse, TexCoords));
		vec3 specular = light[i].specular * spec * vec3(texture(material.specular, TexCoords));
		
		ivec3 type = ivec3(light[i].type);
		
		ambient = ambient * pow(attenuation, type[1] & type[2]) * pow(intensity, type[1] | type[2]);
		diffuse = diffuse * pow(attenuation, type[1] & type[2]) * pow(intensity, type[1] | type[2]);
		specular = specular * pow(attenuation, type[1] & type[2]) * pow(intensity, type[1] | type[2]);		

//		ambient *= mix(vec3(1,1,1), material.color, mask);
//		diffuse *= mix(vec3(1,1,1), material.color, mask);

		result += (ambient + diffuse + specular);
	}
	FragColor = vec4(result, 1.0);
} 