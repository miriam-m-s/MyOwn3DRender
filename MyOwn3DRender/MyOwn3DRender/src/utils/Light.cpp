#include"Light.h"


glm::vec3 Light::getAmbient() const {
    return ambient;
}

glm::vec3 Light::getDiffuse() const {
    return diffuse;
}

glm::vec3 Light::getSpecular() const {
    return specular;
}

void Light::setAmbient(const glm::vec3& value) {
    ambient = value;
}

void Light::setDiffuse(const glm::vec3& value) {
    diffuse = value;
}

void Light::setSpecular(const glm::vec3& value) {
    specular = value;
}

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

glm::vec3 DirectionalLight::getDirection() const {
    return direction;
}


void DirectionalLight::setDirection(const glm::vec3& value) {
    direction = value;
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
glm::vec3 PointLight::getPosition() const { return position; }
float PointLight::getConstant() const { return constant; }
float PointLight::getLinear() const { return linear; }
float PointLight::getQuadratic() const { return quadratic; }

// --- Setters ---
void PointLight::setPosition(const glm::vec3& value) { position = value; }
void PointLight::setConstant(float value) { constant = value; }
void PointLight::setLinear(float value) { linear = value; }
void PointLight::setQuadratic(float value) { quadratic = value; }