#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
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
	
	ivec3 type;
};

#define NUM_OF_LIGHTS 2

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light[NUM_OF_LIGHTS];

vec3 dirLight(vec3 norm, vec3 viewDir, int index)
{
    vec3 lightDir = normalize(-light[index].direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light[index].ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light[index].diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    vec3 specular = light[index].specular * spec * texture(material.specular, TexCoords).rgb;  
	
	return ambient + diffuse + specular;
}

vec3 pointLight(vec3 norm, vec3 viewDir, int index)
{
	vec3 lightDir = normalize(light[index].position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	float distance = length(light[index].position - FragPos);
    float attenuation = 1.0 / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));    

    vec3 ambient = light[index].ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light[index].diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light[index].specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 spotLight(vec3 norm, vec3 viewDir, int index)
{
	vec3 lightDir = normalize(light[index].position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);	
	
	float distance = length(light[index].position - FragPos);
    float attenuation = 1.0 / (light[index].constant + light[index].linear * distance + light[index].quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light[index].direction)); 
    float epsilon = light[index].cutOff - light[index].outerCutOff;
    float intensity = clamp((theta - light[index].outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light[index].ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light[index].diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light[index].specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return ambient + diffuse + specular;
	
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	
        
    vec3 result;
	
result += dirLight(norm, viewDir, 0);
//result += pointLight(norm, viewDir, 1);
	
	for(int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		//result = dirLight(norm, viewDir, i);
		
		//result = pointLight(norm, viewDir, i);
		
		//result += spotLight(norm, viewDir, i);
	}
	
	
	
    FragColor = vec4(result, 1.0);
} 