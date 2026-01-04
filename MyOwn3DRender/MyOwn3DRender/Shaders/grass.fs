#version 330 core

struct Material {
    sampler2D texture_diffuse;
};

in vec2 TexCoord;
in vec4 vertexColor;

uniform Material material;
uniform sampler2D mask;

out vec4 FragColor;

void main()
{
    vec4 maskTex = texture(mask, TexCoord);

    // MASKED (alpha cutout)
    if (maskTex.a < 0.5)
        discard;

    vec3 baseColor = texture(material.texture_diffuse, TexCoord).rgb;

  

    FragColor = vec4(baseColor, 1.0);
}
