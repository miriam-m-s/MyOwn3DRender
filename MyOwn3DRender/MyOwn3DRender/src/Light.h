#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Light {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light() = default;
    virtual ~Light() = default;

    virtual void applyToShader(Shader& shader, int index = 0) const = 0;
};
class DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight(const glm::vec3& dir,
        const glm::vec3& amb,
        const glm::vec3& diff,
        const glm::vec3& spec);

    void applyToShader(Shader& shader, int index = 0) const override;
};
class PointLight : public Light {
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(const glm::vec3& pos,
               const glm::vec3& amb,
               const glm::vec3& diff,
               const glm::vec3& spec,
               float c, float l, float q);

    void applyToShader(Shader& shader, int index) const override;
};