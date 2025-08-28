#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ----- Colores y vectores básicos -----
    vec3 lightColor = vec3(1.0); // luz blanca
    vec3 norm       = normalize(Normal);
    vec3 lightDir   = normalize(lightPos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // ----- Componente especular -----
    float specularStrength = 0.5;
    float shininess = 256.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // ----- Componente difusa -----
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // ----- Componente ambiental -----
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // ----- Color final con texturas -----
    vec4 texColor = mix(texture(texture1, TexCoord),
                        texture(texture2, TexCoord),
                        0.2);

    vec3 lighting = ambient + diffuse + specular;
    FragColor = texColor * vec4(lighting, 1.0);
}
