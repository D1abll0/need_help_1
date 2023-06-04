#version 150 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular; 
	sampler2D mask;
	sampler2D emission;
    float shininess;
	vec3 color;
}; 

struct Light {
    vec3 position;
	vec3 direction;
	
	float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
	
	int type;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 2

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light[2];
uniform SpotLight spotLight;

float constant = 1.0; 

void main()
{    
Light light = light[0];

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
	vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	
	vec3 mask = vec3(texture(material.mask, TexCoords));

//	diffuse *= vec3(1,0,0); //light color
//	specular *= vec3(1,0,0); //light color

//if(mask != vec3(1))
//{
//	ambient *= material.color;
//	diffuse *= material.color;
//}


vec3 color = vec3(1, 0, 1);

ambient *= mix(vec3(1), color, mask);
diffuse *= mix(vec3(1), color, mask);





    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}