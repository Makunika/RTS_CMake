#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
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
};

#define MAX_LIGHTS 32

uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform DirLight dirLight;

uniform int countPointLights;
uniform int countSpotLights;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess;
uniform vec3 viewPos;

vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diff_t, vec3 spec_t);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t);

void main()
{
    vec3 norm = texture(texture_normal1, TexCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(TBN * norm);
    vec3 diff = vec3(texture(texture_diffuse1,TexCoords));
    vec3 spec = vec3(texture(texture_specular1, TexCoords));
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    //vec3 result = calcDirLight(dirLight, norm, viewDir, diff, spec);

    /*for(int i = 0; i < countPointLights; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir, diff, spec);
    }*/

    for(int i = 0; i < countSpotLights; i++) {
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir, diff, spec);
    }

    FragColor = vec4(result, 1.0);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t)
{
    // Окружающая составляющая
    vec3 ambient = light.ambient * diff_t;

    // Диффузная составляющая
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diff_t;

    // Отраженная составляющая
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * spec_t;

    // Прожектор
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // Затухание
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diff_t, vec3 spec_t)
{
    vec3 lightDir = normalize(-light.direction);

    // Рассеянное затенение
    float diff = max(dot(normal, lightDir), 0.0);

    // Отраженное затенение
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // Комбинируем результаты
    vec3 ambient = light.ambient * diff_t;
    vec3 diffuse = light.diffuse * diff * diff_t;
    vec3 specular = light.specular * spec * spec_t;
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t)
{

    vec3 ambient = light.ambient * diff_t;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diff_t);

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0f);
    vec3 specular = light.specular * spec * spec_t;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
