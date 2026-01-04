#version 330 core
struct Material {
    sampler2D texture_diffuse;
};
//in
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec4 vertexColor;



uniform Material material;


out vec4 FragColor;


void main()
{
    
    vec3 diffuseColor = texture(material.texture_diffuse, TexCoord).rgb;
   
   
    FragColor = vec4(diffuseColor, 1.0);
  

}
