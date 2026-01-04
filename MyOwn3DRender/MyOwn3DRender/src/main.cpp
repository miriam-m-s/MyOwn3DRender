#include "Renderer.h"
#include"Model.h"
#include "Shader.h"
#include "Renderable.h"
#include "Camera.h"
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
    
    Shader ourShader("Shaders/bee.vs", "Shaders/bee.fs");
    Shader sunflower("Shaders/sunflower.vs", "Shaders/sunflower.fs");
    Shader unlit("Shaders/unlit.vs", "Shaders/unlit.fs");
    Shader grassSh("Shaders/grass.vs", "Shaders/grass.fs");
 
    Texture* textureNoise = new Texture("assets/textures/noise.png", GL_RGBA, false);
    textureNoise->type = "noiseTex";
    Texture* textureMask = new Texture("assets/scene/GrassAlpha.png", GL_RGBA, false);
    textureMask->type = "mask";
    Model* modelo = new Model("assets/bee/bee.obj", &ourShader);
    Model* ground = new Model("assets/scene/ground.obj");
    Model* sky = new Model("assets/scene/sky.obj", &unlit);
    Model* grass = new Model("assets/scene/grass.obj", &grassSh, { *textureNoise,*textureMask });
    
    Model* sunModel = new Model("assets/scene/sunflower.obj", &sunflower, { *textureNoise });
   


    auto dir = new DirectionalLight(
        glm::vec3(-0.1f, -1.0f, -0.6f),
        glm::vec3(1.1,1.05,1),
        glm::vec3(0.8f),
        glm::vec3(1.0f)
    );
   Renderer::Instance()->setDirectionalLight(dir);
   Renderer::Instance()->getCamera()->setPosition({ -3,6,16 });


   Renderer::addRenderable(modelo); 
   Renderer::addRenderable(sunModel);
   Renderer::addRenderable(sky); 
   Renderer::addRenderable(grass);
   Renderer::addRenderable(ground);

	Renderer::RenderLoop();
	Renderer::Release();
    delete textureNoise;
    delete textureMask;
}