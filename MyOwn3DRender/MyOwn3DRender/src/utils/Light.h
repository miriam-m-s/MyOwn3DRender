#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Light {
public:
    Light() = default;
    // Getters
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;

    // Setters
    void setAmbient(const glm::vec3& value);
    void setDiffuse(const glm::vec3& value);
    void setSpecular(const glm::vec3& value);
    
    virtual ~Light() = default;

    virtual void applyToShader(Shader& shader, int index = 0) const = 0;
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
class DirectionalLight : public Light {
public:
 

    DirectionalLight(const glm::vec3& dir,
        const glm::vec3& amb,
        const glm::vec3& diff,
        const glm::vec3& spec);

    void applyToShader(Shader& shader, int index = 0) const override;
    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3& value);
private:
    glm::vec3 direction;
};
class PointLight : public Light {
public:
 

    PointLight(const glm::vec3& pos,
               const glm::vec3& amb,
               const glm::vec3& diff,
               const glm::vec3& spec,
               float c, float l, float q);

    void applyToShader(Shader& shader, int index) const override;

    glm::vec3 getPosition() const;
    float getConstant() const;
    float getLinear() const;
    float getQuadratic() const;

    void setPosition(const glm::vec3& value);
    void setConstant(float value);
    void setLinear(float value);
    void setQuadratic(float value);
private :
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};