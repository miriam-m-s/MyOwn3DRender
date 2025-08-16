#include "Renderable.h"
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include"Shader.h"
#include"Texture.h"
#include <glm/gtc/type_ptr.hpp>
// ------------------------- Geometría -------------------------
//Concepto	Resumen claro
//VBO	Guarda los datos de los vértices(ej.posiciones)
//VAO	Guarda la configuración para interpretar los VBOs
//EBO  stores indices that OpenGL uses to decide what vertices to draw
Cube::Cube(Shader* shader, Texture* texture1, Texture* texture2):sh1(shader),t1(texture1),t2(texture2)
{
    //float vertices[] = {
    //   //position          //color              //texture coords
    //   0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
    //   0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,// bottom right
    //  -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    0.0f, 0.0f,// bottom left
    //  -0.5f,  0.5f, 0.0f , 1.0f, 0.0f, 0.0f,    0.0f, 1.0f// top left 
    //};
    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    float vertices[] = {
        //pos                   //texcoords
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };




    //glGenBuffers(1, &EBO);
    //  Crear un Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);   // El VAO guardará la configuración de los atributos
    //  Crear un Vertex Buffer Object(VBO)
    glGenBuffers(1, &VBO);        // El VBO almacenará los datos reales de los vértices

    // Enlazar el VAO (cualquier configuración después de aquí se guardará en él)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// Elegimos el tipo de buffer (ARRAY para vértices)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // - GL_STATIC_DRAW indica que los datos no cambiarán (solo se usan para dibujar)

  // 6 Configurar el atributo de vértice
  // - location = 0 → debe coincidir con el layout en el shader
  // - 3 → 3 componentes por vértice (x, y, z)
  // - GL_FLOAT → tipo de cada componente
  // - GL_FALSE → no normalizar
  // - stride = 3 * sizeof(float) → separación entre vértices
  // - (void*)0 → desplazamiento inicial (inicio del array)
    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);*/

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    sh1->use();
    glUniform1i(glGetUniformLocation(sh1->ID, "texture1"), 0);
    sh1->setInt("texture2", 1);
    glUniformMatrix4fv(glGetUniformLocation(sh1->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

}

void Cube::draw(const glm::mat4& view, const glm::mat4& projection)
{
    sh1->use();
    glUniformMatrix4fv(glGetUniformLocation(sh1->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(sh1->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(sh1->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


    t1->bind(GL_TEXTURE0);
    t2->bind(GL_TEXTURE1);

    glBindVertexArray(VAO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0); // opcional, por orden
}
