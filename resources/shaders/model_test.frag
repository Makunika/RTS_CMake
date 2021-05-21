//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//in vec3 Normal;
//in vec3 FragPos;
//in mat3 TBN;
//
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;
//
//struct PointLight {
//    vec3 position;
//
//    float constant;
//    float linear;
//    float quadratic;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct SpotLight {
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//
//    float constant;
//    float linear;
//    float quadratic;
//};
//
//struct DirLight {
//    vec3 direction;
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//struct Material {
//    float shininess;
//};
//
//#define NR_POINT_LIGHTS 4
//
//uniform vec3 viewPos;
//uniform DirLight dirLight;
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight;
//uniform Material material;
//
//vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);
//
//    // Рассеянное затенение
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    // Отраженное затенение
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    // Комбинируем результаты
//    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
//    return (ambient + diffuse + specular);
//}
//
//vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
//    // Окружающая составляющая
//    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
//
//    // Диффузная составляющая
//    vec3 lightDir = normalize(light.position - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
//
//    // Отраженная составляющая
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;
//
//    // Прожектор
//    float theta = dot(lightDir, normalize(-light.direction));
//    float epsilon = (light.cutOff - light.outerCutOff);
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    diffuse  *= intensity;
//    specular *= intensity;
//
//    // Затухание
//    float distance = length(light.position - FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//
//    return (ambient + diffuse + specular);
//}
//
//vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//
//    // Рассеянное затенение
//    float diff = max(dot(normal, lightDir), 0.0);
//
//    // Отраженное затенение
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    // Затухание
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//    // Комбинируем результаты
//    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//}
//
//void main()
//{
////    vec3 norm = texture(texture_normal1, TexCoords).rgb;
////    norm = norm * 2.0 - 1.0;
////    norm = normalize(TBN * norm);
//    vec3 norm = normalize(Normal);
//    vec3 viewDir = normalize(viewPos - FragPos);
//
//    // Этап №1: Направленное освещение
//    vec3 result = calcDirLight(dirLight, norm, viewDir);
//
//    // Этап №2: Точечные источники света
////    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
////        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
////    }
//
//    // Этап №3: Прожектор
//    //result += calcSpotLight(spotLight, norm, FragPos, viewDir);
//
//    FragColor = vec4(result, 1.0);
//}
//
//

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

uniform PointLight pointLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess;
uniform vec3 viewPos;


void main()
{
    vec3 ambient = pointLight.ambient * vec3(texture(texture_diffuse1,TexCoords));
    vec3 norm = texture(texture_normal1, TexCoords).rgb;
    norm = norm * 2.0 - 1.0;
    norm = normalize(TBN * norm);

    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * vec3(texture(texture_diffuse1,TexCoords)));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0f);
    vec3 specular = pointLight.specular * spec* (vec3(texture(texture_specular1,TexCoords)));

    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}
