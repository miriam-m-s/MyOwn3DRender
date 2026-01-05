#version 330 core

// ---------- Salida ----------
out vec4 FragColor;

// ---------- Entradas ----------
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// ---------- Estructuras ----------
struct Material {
    sampler2D texture_diffuse;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

// ---------- Uniforms ----------
#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

// ---------- Funciones ----------

// Luz direccional
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);

    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    // Componentes
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return ambient + diffuse + specular;
}

// Luz puntual
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient  = light.ambient  * attenuation;
    vec3 diffuse  = light.diffuse  * diff * attenuation;
    vec3 specular = light.specular * spec * attenuation;

    return ambient + diffuse + specular;
}

// ---------- Main ----------
void main()
{
    // Normal y vector hacia la cámara
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Luz direccional
    vec3 lighting = CalcDirLight(dirLight, norm, viewDir);

    // Luces puntuales
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        lighting += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    // Color de la textura
    vec3 diffuseColor = texture(material.texture_diffuse, TexCoord).rgb;

    // Color final
    vec3 finalColor = lighting * diffuseColor;

    FragColor = vec4(lighting, 1.0);
}
