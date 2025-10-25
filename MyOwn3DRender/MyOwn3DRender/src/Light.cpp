#include"Light.h"
DirectionalLight::DirectionalLight(const glm::vec3& dir,
    const glm::vec3& amb,
    const glm::vec3& diff,
    const glm::vec3& spec)
{
    direction = dir;
    ambient = amb;
    diffuse = diff;
    specular = spec;
}

void DirectionalLight::applyToShader(Shader& shader, int index) const
{
    shader.setVec3("dirLight.direction", direction);
    shader.setVec3("dirLight.ambient", ambient);
    shader.setVec3("dirLight.diffuse", diffuse);
    shader.setVec3("dirLight.specular", specular);
}
PointLight::PointLight(const glm::vec3& pos,
    const glm::vec3& amb,
    const glm::vec3& diff,
    const glm::vec3& spec,
    float c, float l, float q)
{
    position = pos;
    ambient = amb;
    diffuse = diff;
    specular = spec;
    constant = c;
    linear = l;
    quadratic = q;
}

void PointLight::applyToShader(Shader& shader, int index) const

{
   
  
    std::string prefix = "pointLights[" + std::to_string(index) + "]";
    shader.setVec3(prefix + ".position", position);
    shader.setVec3(prefix + ".ambient", ambient);
    shader.setVec3(prefix + ".diffuse", diffuse);
    shader.setVec3(prefix + ".specular", specular);
    shader.setFloat(prefix + ".constant", constant);
    shader.setFloat(prefix + ".linear", linear);
    shader.setFloat(prefix + ".quadratic", quadratic);
}