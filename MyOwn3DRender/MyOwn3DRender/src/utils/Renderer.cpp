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
    if (renderInstance->depthShader) {
        delete renderInstance->depthShader;
        renderInstance->depthShader = nullptr;
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

    renderInstance->depthShader=new Shader("Shaders/ShadowDepth.vs", "Shaders/ShadowDepth.fs");
    glGenFramebuffers(1, &renderInstance->depthMapFBO);


    glGenTextures(1, &renderInstance->depthMap);
    glBindTexture(GL_TEXTURE_2D, renderInstance->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        renderInstance->SHADOW_WIDTH, renderInstance->SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, renderInstance->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderInstance->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
void Renderer::ApplyFixedViewport()
{
    float targetAspect = (float)LOGICAL_WIDTH / LOGICAL_HEIGHT;
    float windowAspect = (float)width / height;

    int vpWidth, vpHeight;
    int vpX = 0, vpY = 0;

    if (windowAspect > targetAspect)
    {
        // Ventana más ancha → barras laterales
        vpHeight = height;
        vpWidth = int(height * targetAspect);
        vpX = (width - vpWidth) / 2;
    }
    else
    {
        // Ventana más alta → barras arriba/abajo
        vpWidth = width;
        vpHeight = int(width / targetAspect);
        vpY = (height - vpHeight) / 2;
    }

    glViewport(vpX, vpY, vpWidth, vpHeight);
}

void Renderer::loopPrivate()
{

    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glm::mat4 lightSpaceMatrix = computeLightSpaceMatrix();
       
        // 1️⃣ Primera pasada — render al depth map
       
        int prevViewport[4];
        glGetIntegerv(GL_VIEWPORT, prevViewport);

        // --- SHADOW PASS ---
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        glCullFace(GL_FRONT);
        renderInstance->depthShader->use();
        renderInstance->depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
  
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        for (auto& rObjects : renderInstance->renderObjects)
        {
            renderInstance->depthShader->setMat4("model", glm::mat4(1.0f));
            rObjects->Draw(*renderInstance->depthShader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);
        /*glViewport(prevViewport[0], prevViewport[1],
            prevViewport[2], prevViewport[3]);
        ApplyFixedViewport();*/
        glViewport(0, 0, renderInstance->width, renderInstance->height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderInstance->view = glm::lookAt(cam->getPosition(), cam->getPosition() + cam->getFront(), cam->getUp());
      
        float aspect = static_cast<float>(renderInstance->width) / static_cast<float>(renderInstance->height);
        renderInstance->projection =
            glm::perspective(glm::radians(cam->getFov()),
                aspect,
                0.1f,
                100.0f);


     
        for (auto& rObjects : renderInstance->renderObjects) {
            
            Shader* sh = rObjects->getShader();
            if (!sh)sh = renderInstance->ourShader;
            sh->use();

            // Luz direccional
            if (renderInstance->dirLight)
                renderInstance->dirLight->applyToShader(*sh);
       
            // Luces puntuales
            sh->setInt("nPointLights", renderInstance->pointLights.size());
           
            for (int i = 0; i < renderInstance->pointLights.size(); ++i)
                renderInstance->pointLights[i]->applyToShader(*renderInstance->ourShader, i);

            // Posición del observador
            sh->setFloat("time", glfwGetTime());
            sh->setVec3("viewPos", renderInstance->cam->getPosition());
            sh->setMat4("view", renderInstance->view);
            sh->setMat4("projection", renderInstance->projection);
            sh->setMat4("lightSpaceMatrix", lightSpaceMatrix);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, renderInstance->depthMap);
       
            sh->setInt("shadowMap", 1);
            rObjects->Draw(*sh);
            
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
glm::mat4 Renderer::computeLightSpaceMatrix()
{
    glm::vec3 lightDir = renderInstance->dirLight->getDirection();
    float near_plane = 1.0f, far_plane = 50.f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(-lightDir * 20.0f, glm::vec3(0.0f), glm::vec3(0, 1, 0));

    return lightProjection * lightView;
}
void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
     
  

    renderInstance->width = width;
    renderInstance->height = height;

    glViewport(0, 0, width, height);
    std::cout << "reescalando" << std::endl;
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
    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}


