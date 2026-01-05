#version 330 core

struct Material {
    sampler2D texture_diffuse;
};
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec2 TexCoord;
in vec4 vertexColor;

uniform Material material;
uniform sampler2D mask;
uniform DirLight dirLight;
out vec4 FragColor;

void main()
{
    vec4 maskTex = texture(mask, TexCoord);

    // MASKED (alpha cutout)
    if (maskTex.a < 0.5)
        discard;

    vec3 baseColor = texture(material.texture_diffuse, TexCoord).rgb;

  

    FragColor = vec4(baseColor*dirLight.ambient, 1.0);
}
