#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 FragPosLightSpace;
out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform float time;
uniform sampler2D noiseTex;

void main()
{
    vec3 pos = aPos;

    // Máscara desde vertex color (alas)
    float mask = aColor.r;

    // UV animadas para la noise
    vec2 noiseUV = vec2(pos.x,pos.y) / 20.0 + vec2(time * 0.5, time * 0.2);

    // Samplear noise (0–1)
    float noiseValue = texture(noiseTex, noiseUV).r;

    // Convertir a rango -1 a 1
    noiseValue = noiseValue * 2.0 - 1.0;

    // Intensidad del movimiento
    float wingOffset = noiseValue * 0.25;

    // Aplicar solo a las alas
    pos.y += wingOffset * mask;

    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoords;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
    vertexColor = aColor;
}
