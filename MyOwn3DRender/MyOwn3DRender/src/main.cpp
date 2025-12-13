#include "Renderer.h"
#include "Texture.h"
#include"Model.h"
#include "Shader.h"
#include "Renderable.h"
#include <iostream>
#include <assimp/config.h>
#include "Light.h"
#include<string>

int main() {
	if (!Renderer::Init(800, 600, "Ventana OpenGL")) {
		std::cerr << "Error de inicializacion de renderer" << std::endl;
		return 1;
	}
    // unsigned int shaderProgram = createShaderProgram();
    Shader ourShader("Shaders/ModelVertex.vs", "Shaders/ModelFragment.fs");


   
    Model* modelo = new Model("assets/bee/bee.obj");
    Model* plano = new Model("assets/plano/plano.obj");
    Model* plano1 = new Model("assets/plano/plano1.obj");
   /* while (true) {
        modelo->Draw(ourShader);
    }
    Texture texture1("assets/textures/container.jpg");*/

    auto dir = new DirectionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(1,0.9,0.6),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    );
    Renderer::Instance()->setDirectionalLight(dir);

    auto point = new PointLight(
        glm::vec3(2.f, 1.4f, 3.0f),
        glm::vec3(0.05f),
        glm::vec3(1.0f, 0.2f, 1.0f),
        glm::vec3(1.0f),
       1.f, 0.14f, 0.07f
    );
    Renderer::Instance()->addPointLight(point);
    auto point2 = new PointLight(
        glm::vec3(-2.f, 0.7f, -2.0f),
        glm::vec3(0.05f),
        glm::vec3(0.0f, 0.7f, 1.0f),
        glm::vec3(1.0f),
        1.f, 0.14f, 0.07f
    );
    Renderer::Instance()->addPointLight(point2);

    //Texture texture2("assets/textures/ponch.png", GL_RGBA);

 
    //Cube* cube = new Cube(&ourShader, &texture1, &texture2);
    Renderer::addRenderable(modelo);
    Renderer::addRenderable(plano);
    Renderer::addRenderable(plano1);
    // Loop de renderizado
	Renderer::RenderLoop();
	Renderer::Release();
}