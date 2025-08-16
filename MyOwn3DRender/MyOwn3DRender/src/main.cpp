#include"Renderer.h"
#include"Texture.h"
#include"Shader.h"
#include "Renderable.h"
#include <iostream>
int main() {
	if (!Renderer::Init(800, 600, "Ventana OpenGL")) {
		std::cerr << "Error de inicializacion de renderer" << std::endl;
		return 1;
	}

    Texture texture1("assets/textures/container.jpg");


    Texture texture2("assets/textures/ponch.png", GL_RGBA);

    // unsigned int shaderProgram = createShaderProgram();
    Shader ourShader("Shaders/VertexShader.vs", "Shaders/VertexShader.fs");
    Cube* cube = new Cube(&ourShader, &texture1, &texture2);
    Renderer::addRenderable(cube);
    // Loop de renderizado
	Renderer::RenderLoop();
	Renderer::Release();
}