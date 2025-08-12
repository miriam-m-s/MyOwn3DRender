#include <glad/glad.h> //carga funciones de opengl
#include <GLFW/glfw3.h>//maneja creacion de ventanas input...
#include <iostream>
#include "Shader.h"
#include <direct.h> 
#include "stb_image.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw= -90;
float pitch = 0;
bool firstMouse = true;
bool clicked = false;
float fov = 45;

float lastX = 400, lastY = 300;
// ------------------------- Configuración -------------------------
//se llama automáticamente cada vez que se redimensiona la ventana, y ajusta el área de dibujo (glViewport) para que coincida con el nuevo tamaño.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
//manejo de la entrada del teclado
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        clicked = true;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        clicked = false;
    }
}

bool initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW\n";
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}
//creacion de la ventana
GLFWwindow* createWindow(int width, int height, const char* title) {
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

bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }
    return true;
}

// ------------------------- Shaders -------------------------

// Función: compileShader
// Descripción: Compila un shader (de vértice o fragmento) a partir de su código fuente.
// 
// Parámetros de entrada:
//   - type: Tipo de shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER)
//   - source: Código fuente GLSL del shader (como cadena de texto)
//
// Valor de salida:
//   - Retorna el ID (unsigned int) del shader compilado (aunque puede estar mal compilado si hubo error)
//
// -----------------------------------------------------------------------------
unsigned int compileShader(unsigned int type, const char* source) {
    //Devuelve un identificador del shader vacio 
    unsigned int shader = glCreateShader(type);
    //carga el codigo fuente del shader
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error al compilar shader:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int createShaderProgram() {
    //codigo fuente para el vertex shader 
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor; // output a color to the fragment shader \n"
        "void main() {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor; \n"
        "}\0";
    //codigo fuente para el fragment shader
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "void main() {\n"
        "    FragColor = vec4(ourColor, 1.0f);\n"
        "}\0";

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    //crear el shader program 
    unsigned int shaderProgram = glCreateProgram();
    //adjuntar los shader compiladoes al programa
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //enlazar el programa (une ambos shaders en uno)
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error al enlazar shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// ------------------------- Geometría -------------------------
//Concepto	Resumen claro
//VBO	Guarda los datos de los vértices(ej.posiciones)
//VAO	Guarda la configuración para interpretar los VBOs
//EBO  stores indices that OpenGL uses to decide what vertices to draw
void createTriangleData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {
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
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!clicked) {
        firstMouse = true;
        return;
    }
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float offsetX = xpos - lastX;
    float offsetY = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;
    yaw += offsetX;
    pitch += offsetY;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
// ------------------------- Programa principal -------------------------

int main() {

    if (!initGLFW()) return -1;

    GLFWwindow* window = createWindow(800, 600, "Ventana OpenGL");
    if (!window) return -1;

    if (!initGLAD()) return -1;

    glViewport(0, 0, 800, 600);

    unsigned int VAO, VBO,EBO;
    createTriangleData(VAO, VBO,EBO);
   
    Texture texture1("assets/textures/container.jpg");
      

    Texture texture2("assets/textures/ponch.png", GL_RGBA);
       
   // unsigned int shaderProgram = createShaderProgram();
    Shader ourShader("Shaders/VertexShader.vs", "Shaders/VertexShader.fs");
    // Loop de renderizado
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    /*glm::mat4 view = glm::mat4(1.0f);*/
    // note that we're translating the scene in the reverse direction of where we want to move
    const float radius = 10.0f;
    
    glm::mat4 projection;
    ;
   
    glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    //camara 
   /* glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);*/
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       /* model= glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        */
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
       // glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        ourShader.use();
        texture1.bind(GL_TEXTURE0);
        texture2.bind(GL_TEXTURE1);

        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
