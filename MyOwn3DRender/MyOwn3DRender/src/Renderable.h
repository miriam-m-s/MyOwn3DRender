#pragma once
#include <glm/glm.hpp>
class Texture;
class Shader;
class Renderable {
public:
    virtual ~Renderable() = default;

    // Método obligatorio que todo objeto debe implementar
    virtual void draw(const glm::mat4& view, const glm::mat4& projection) = 0;

    // Opcional: actualizar lógica/animación
    virtual void update(float deltaTime) {}
protected:
    // Cada objeto puede tener su propia transform
    glm::mat4 model = glm::mat4(1.0f);
    unsigned int VAO, VBO, EBO;
};
class Cube :public Renderable {
public:    
    Cube(Shader* shader,Texture* texture1,Texture* texture2);
    void draw(const glm::mat4& view, const glm::mat4& projection) override;
private:
    Texture* t1;
    Texture* t2;
    Shader* sh1;
};