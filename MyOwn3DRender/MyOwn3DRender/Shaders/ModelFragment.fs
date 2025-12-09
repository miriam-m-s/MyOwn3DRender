#version 330 core

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular;
    sampler2D texture_specular1;
    float shininess;
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

// ----- Uniforms -----
#define NR_POINT_LIGHTS 4  // puedes cambiarlo según tus necesidades

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int nPointLights=0;
out vec4 FragColor;
float remap(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
         vec3 lightDir = normalize(-light.direction);
    
        // Difusa normalizada 0–1
        float diff = (dot(normal, lightDir) + 1.0) * 0.5;
    
        // Niveles de cel shading
      
        float sombra = smoothstep(0.32, 0.35, diff);
        float luz    = smoothstep(0.95, 0.98, diff);
    
        // Combinamos niveles:
        // - Si diff < 0.33 → 0
        // - Entre 0.33 y 0.75 → 0.5
        // - > 0.75 → 1.0
        float toon =
              0.5 * (1.0 - sombra) +       // sombra
              1 * (sombra - luz) +       // medio tono
              1.5 * luz;                   // luz fuerte
    
        vec3 color =
            light.ambient *
            light.diffuse * toon;
    
        return color;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Distancia a la luz
    float dist = length(light.position - fragPos);

    // Atenuación por distancia

    float constant  = light. constant;
    float linear    = light.linear;
    float quadratic = light.quadratic;

    float attenuation = 1.0 / (constant + linear * dist + quadratic * dist * dist);

    // Difusa normalizada 0–1
    float diff = (dot(normal, lightDir) + 1.0) * 0.5;

    // Aplica atenuación
    diff *= attenuation;

    // Toon smoothing
    float sombra = smoothstep(0.30, 0.35, diff);
    float luz    = smoothstep(0.95, 1.0, diff);

    float toon =
               0.0 * (1.0 - sombra) +      // sombra
               0.5 * (sombra - luz) +      // medio tono
               1.0 * luz;                  // luz fuerte

    vec3 diffuse = light.diffuse * toon;

    return diffuse;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < nPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    //// --- Aplica cel shading ---
    //float intensity = length(result); // luz total

    //// Cuantización (3 niveles)
    //float toonStep = 0.5;
    //float levels = floor(intensity / toonStep);
    //float quantized = levels * toonStep;

    //// Reasignar la luz cuantizada
    //result = normalize(result) * quantized;

    vec3 diffuseColor = texture(material.texture_diffuse, TexCoord).rgb;

    FragColor = vec4( result*diffuseColor, 1.0);
}
