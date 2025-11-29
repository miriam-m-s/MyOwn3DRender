#include"Renderer.h"
#include"Texture.h"
#include"Shader.h"
#include "Renderable.h"
#include <iostream>
#include<assimp/config.h>
#include"Light.h"
int main() {
	if (!Renderer::Init(800, 600, "Ventana OpenGL")) {
		std::cerr << "Error de inicializacion de renderer" << std::endl;
		return 1;
	}

    Texture texture1("assets/textures/container.jpg");

    auto dir = new DirectionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.2f),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    );
    Renderer::Instance()->setDirectionalLight(dir);

    auto point = new PointLight(
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(0.05f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f),
        1.0f, 0.09f, 0.032f
    );
    Renderer::Instance()->addPointLight(point);

    Texture texture2("assets/textures/ponch.png", GL_RGBA);

    // unsigned int shaderProgram = createShaderProgram();
    Shader ourShader("Shaders/VertexShader.vs", "Shaders/VertexShader.fs");
    Cube* cube = new Cube(&ourShader, &texture1, &texture2);
    Renderer::addRenderable(cube);
    // Loop de renderizado
	Renderer::RenderLoop();
	Renderer::Release();
}