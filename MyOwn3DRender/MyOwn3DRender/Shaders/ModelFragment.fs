#version 330 core

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular;
    sampler2D texture_specular1;
    float shininess;
};

uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec2 texCoordsFlipped = vec2(TexCoord.x, 1.0 - TexCoord.y);
    // --- Difuse texture ---
    vec3 diffuseColor = texture(material.texture_diffuse, texCoordsFlipped).rgb;
    FragColor = vec4(diffuseColor, 1.0);
    //// --- Ambient light ---
    //vec3 ambient = 0.1 * diffuseColor;

    //// --- Diffuse light ---
    //vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * diffuseColor;

    //// --- Specular ---
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = spec * texture(material.texture_specular, TexCoords).rgb;

    //vec3 result = ambient + diffuse + specular;
   
}
