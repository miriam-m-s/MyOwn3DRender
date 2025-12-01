#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>
struct GLFWwindow;
class Shader;
class Texture;
class Camera;
class Renderable;
class DirectionalLight;
class PointLight;
class  Renderer {
public:

	/// <summary>
	/// Devuelve la instancia del singleton
	/// </summary>
	/// <returns>Instancia del singleton</returns>
	static Renderer* Instance();
	/// <summary>
	/// Metodo para la "destructora" de Renderer
	/// </summary>
	static void Release();
	/// <summary>
	/// Metodo para inicializar de Renderer
	/// </summary>
	static bool Init(int w, int h, const char* name);
	
	static void RenderLoop();
	static void addRenderable(Renderable* obj);
	Camera* getCamera();
	void setDirectionalLight(DirectionalLight* light);
	void addPointLight(PointLight* light);

private:
	/// <summary>
	/// Inicializacion privada del singleton
	/// </summary>
	/// <returns>Booleano para exito o fracaso</returns>
	bool initPrivate(int w, int h, const char* name);
	bool initGLAD();
	void processInput(GLFWwindow* window);
	void loopPrivate();
	bool initGLFW();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* createWindow(int width, int height, const char* title);
	/// <summary>
	/// Metodo privado para liberacion de recursos del singleton
	/// </summary>
	void releasePrivate();
	Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	

	static std::unique_ptr<Renderer> renderInstance;

	//variables
	int width, height;

	GLFWwindow* window;
	Camera* cam;
	unsigned int VAO, VBO, EBO;
	Shader* ourShader;
	Texture* texture1;
	Texture* texture2;
	DirectionalLight* dirLight = nullptr;
	std::vector<PointLight*> pointLights;
	std::vector<Renderable*>renderObjects;

	glm::mat4 view;
	glm::mat4 projection;

};