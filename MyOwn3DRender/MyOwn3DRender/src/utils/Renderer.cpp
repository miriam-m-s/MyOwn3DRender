#include "Renderer.h"
#include <glad/glad.h> //carga funciones de opengl
#include <GLFW/glfw3.h>//maneja creacion de ventanas input...
#include"Model.h"
#include"Light.h"
#include "MyOwn3DRender.cpp"
#include"Camera.h"
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
std::unique_ptr<Renderer> Renderer::renderInstance = nullptr;
Renderer::Renderer()
    : width(0), height(0), window(nullptr), cam(nullptr),
    VAO(0), VBO(0), EBO(0), ourShader(nullptr),
    texture1(nullptr), texture2(nullptr)
{
    
}
Renderer* Renderer::Instance()
{
	assert(renderInstance);
	return renderInstance.get();
}

void Renderer::Release()
{
    renderInstance->releasePrivate();
}
void Renderer::releasePrivate()
{
    assert(renderInstance);
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    delete renderInstance->cam;
    renderInstance->cam = nullptr;
    for (auto& rObjects : renderInstance->renderObjects) {
        delete rObjects;
        rObjects = nullptr;
    }
  
    // --- 1️⃣ Borrar luces ---
    if (renderInstance->dirLight) {
        delete renderInstance->dirLight;
        renderInstance->dirLight = nullptr;
    }

    for (auto& light : renderInstance->pointLights) {
        delete light;
        light = nullptr;
    }
    if (renderInstance->ourShader) {
        delete renderInstance->ourShader;
        renderInstance->ourShader = nullptr;
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    renderInstance->pointLights.clear();
    renderInstance.reset();
    renderInstance = nullptr;
}

bool Renderer::Init(int w, int h, const char* name)
{
	return renderInstance->initPrivate(w,h,name);
}

void Renderer::RenderLoop()
{
    assert(renderInstance);

    renderInstance->loopPrivate();
}

void Renderer::addRenderable(Model* obj)
{
   renderInstance->renderObjects.push_back(obj);
}

Camera* Renderer::getCamera()
{
    return renderInstance->cam;
}
void Renderer::setDirectionalLight(DirectionalLight* light) {
    renderInstance->dirLight = light;
}

void Renderer::addPointLight(PointLight* light) {
    renderInstance->pointLights.push_back(light);
}
bool Renderer::initPrivate(int w, int h, const char* name)
{

    assert(!renderInstance);
    renderInstance.reset(new Renderer);
    if (!initGLFW()) { 
        renderInstance.reset();
        renderInstance = nullptr;
        return false; }
    renderInstance->width = w;
    renderInstance->height = h;
    renderInstance->window = createWindow(w, h, name);
    if (!renderInstance->window) { 
        renderInstance.reset();
        renderInstance = nullptr;
        return false; }
   
    renderInstance->cam = new Camera(renderInstance->window);
    if (!renderInstance->cam) {
        std::cerr << "Error al crear la camara" << std::endl;
        return false;
    }
  
    if (!initGLAD()) return false;
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);


    renderInstance->ourShader = new Shader("Shaders/ModelVertex.vs", "Shaders/ModelFragment.fs");

    if (!renderInstance->ourShader || renderInstance->ourShader->ID == 0)
    {
        std::cerr << "ERROR: Shader no cargado correctamente" << std::endl;
        return false;
    }

    return true;
}
bool Renderer::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }
    return true;
}
void Renderer:: processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    cam->processInput(window);
}
void Renderer::loopPrivate()
{
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderInstance->view = glm::lookAt(cam->getPosition(), cam->getPosition() + cam->getFront(), cam->getUp());
        renderInstance->projection = glm::perspective(glm::radians(cam->getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

     
        for (auto& rObjects : renderInstance->renderObjects) {
    
            renderInstance->ourShader->use();

            // Luz direccional
            if (renderInstance->dirLight)
                renderInstance->dirLight->applyToShader(*renderInstance->ourShader);
       
            // Luces puntuales
            renderInstance->ourShader->setInt("nPointLights", renderInstance->pointLights.size());
           
            for (int i = 0; i < renderInstance->pointLights.size(); ++i)
                renderInstance->pointLights[i]->applyToShader(*renderInstance->ourShader, i);

            // Posición del observador
            renderInstance->ourShader->setVec3("viewPos", cam->getPosition());
            renderInstance->ourShader->setMat4("view", view);
            renderInstance->ourShader->setMat4("projection", projection);
            renderInstance->ourShader->setVec3("viewPos", renderInstance->getCamera()->getPosition());
        
            rObjects->Draw(*renderInstance->ourShader);
            
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

bool Renderer::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}
void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//creacion de la ventana
GLFWwindow* Renderer::createWindow(int width, int height, const char* title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        const char* description;
        glfwGetError(&description);
        std::cerr << "No se pudo crear la ventana. Error: " << description << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}


