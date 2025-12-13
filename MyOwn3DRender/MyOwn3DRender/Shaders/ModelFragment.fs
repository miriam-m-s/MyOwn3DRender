#version 330 core
#define NR_POINT_LIGHTS 4  
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



//in
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int nPointLights=0;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform sampler2D shadowMap;

out vec4 FragColor;

float remap(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}
float ToonQuantize(float v,float dark)
{
    float t1 = step(0.25, v);
    float t2 = step(0.35, v);
    float t3 = step(0.90, v);

    return
        0.3 * (1.0 - t1) +      // zona oscura
        0.55 * (t1 - t2) +       // tono sombra
        0.8 * (t2 - t3) +       // tono medio
        1.0 * t3;               // luz fuerte
}
float ToonQuantize2(float v,float dark)
{
    float t1 = step(0.25, v);
    float t2 = step(0.35, v);
    float t3 = step(0.90, v);

    return
        0.3 * (1.0 - t1) +      // zona oscura
        0.5 * (t1 - t2) +       // tono sombra
        0.7 * (t2 - t3) +       // tono medio
        1.00 * t3;               // luz fuerte
}

 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
         vec3 lightDir = normalize(-light.direction);
    
        // Difusa normalizada 0–1
        float diff = (dot(normal, lightDir) + 1.0) * 0.5;
    
        // Niveles de cel shading
      
        float sombra = step(0.33,  diff);
        float luz    = step(0.95, diff);
       
         // Especular
         vec3 reflectDir = reflect(-lightDir, normal);
         float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
         spec=step(0.8,spec)*0.5;


        float toon = ToonQuantize(diff,0.1);
        vec3 color =
            light.ambient *
            light.diffuse * toon;
        
        // ---- Rim Light Fresnel ----
          float fresnel = 1.0 - max(dot(normal, viewDir), 0.0);
          fresnel*=diff;
          float rim=step(0.6,fresnel);
          
          vec3 rimColor = vec3(1.0, 1.0, 1.0); // puedes cambiarlo
      // +(rimColor * rim * 0.8)
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
   
    

    float toon =ToonQuantize2(diff,0.2);
             

    vec3 diffuse = light.diffuse * toon;

    return diffuse;
}
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Transformar a coordenadas NDC
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // mapear a [0,1]

    // Obtener profundidad actual y bias
    float currentDepth = projCoords.z;
    vec3 lightDir = normalize(-dirLight.direction);
    float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);

    // PCF 3x3 para suavizar bordes
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // Evitar sombras fuera del far plane
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return step(0.9,shadow)*0.5;
}
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < nPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

  

    vec3 diffuseColor = texture(material.texture_diffuse, TexCoord).rgb;
    float shadow = ShadowCalculation(FragPosLightSpace); 
   FragColor = vec4(diffuseColor*(1-shadow)*result, 1.0);
}
