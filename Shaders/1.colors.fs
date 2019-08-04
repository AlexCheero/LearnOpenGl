#version 330 core

struct Material
{
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D emission;
    sampler2D reflection1;
    float shininess;
};

struct DirLight
{
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
	vec3 direction;
	vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform samplerCube skybox;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 cameraPos;

in vec3 fFragPos;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	///*
    vec3 norm = normalize(fNormal);
	vec3 viewDir = normalize(-fFragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, fFragPos, viewDir);
	result += CalcSpotLight(spotLight, norm, fFragPos, viewDir);

    //FragColor = vec4(result, 1.0);
    //*/

    ///*
    vec3 I = normalize(fFragPos);
    vec3 R = reflect(I, normalize(fNormal));
    //float ratio = 1.00 / 1.52;
    //vec3 R = refract(I, normalize(fNormal), ratio);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
    //*/

    /*
    vec4 reflTxt = texture(material.reflection1, fTexCoords);
    float refl = reflTxt.r;
    result = mix(result, texture(skybox, R).rgb, refl);
    FragColor = vec4(result, 1.0);
    */

    FragColor = vec4(result, 1.0);
    FragColor += texture(skybox, R) * texture(material.reflection1, fTexCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, fTexCoords));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, fTexCoords));
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, fTexCoords));
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, fTexCoords));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, fTexCoords));
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, fTexCoords));
	
	float distance = length(light.position - fragPos);
	float attDenominator = (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float attenuation = 1.0 / max(0.000001, attDenominator);
	
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilion = light.cutOff - light.outerCutOff;
	float intencity = clamp((theta - light.outerCutOff) / epsilion, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, fTexCoords));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse1, fTexCoords)) * intencity;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, fTexCoords)) * intencity;
	
	float distance = length(light.position - fragPos);
	float attDenominator = (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float attenuation = 1.0 / max(0.000001, attDenominator);
	
	return (ambient + diffuse + specular) * attenuation;
}