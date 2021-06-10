#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in mat3 TBN;
in vec4 FragPosLightSpace;

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
uniform sampler2D shadow_map;
uniform float shininess;
uniform vec3 viewPos;

vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diff_t, vec3 spec_t, float shadow);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diff_t, vec3 spec_t);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    vec3 norm = texture(texture_normal1, TexCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(TBN * norm);
    vec3 diff = vec3(texture(texture_diffuse1,TexCoords));
    vec3 spec = vec3(texture(texture_specular1, TexCoords));
    vec3 viewDir = normalize(viewPos - FragPos);

    float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 result = calcDirLight(dirLight, norm, viewDir, diff, spec, shadow);

    for(int i = 0; i < countPointLights; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir, diff, spec);
    }

    for(int i = 0; i < countSpotLights; i++) {
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir, diff, spec);
    }

    FragColor = vec4(result, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Выполняем деление перспективы
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Трансформируем в диапазон [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // Получаем наиболее близкое значение глубины исходя из перспективы глазами источника света (используя в диапазон [0,1] fragPosLight в качестве координат)
    float closestDepth = texture(shadow_map, projCoords.xy).r;

    // Получаем глубину текущего фрагмента исходя из перспективы глазами источника света
    float currentDepth = projCoords.z;

    // Вычисляем смещение (на основе разрешения карты глубины и наклона)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Проверка нахождения текущего фрагмента в тени
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Оставляем значение тени на уровне 0.0 за границей дальней плоскости пирамиды видимости глазами источника света
    if(projCoords.z > 1.0)
    shadow = 0.0;

    return shadow;
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

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diff_t, vec3 spec_t, float shadow)
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
    return (ambient + (1.0 - shadow) * (diffuse + specular));
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
