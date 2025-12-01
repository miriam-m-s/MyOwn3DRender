#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::string vertexCode;
	std::string fragmentCode;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//reads file content into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vShaderCode);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderCode);
	//crear el shader program 
	ID = glCreateProgram();
	//adjuntar los shader compiladoes al programa
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//enlazar el programa (une ambos shaders en uno)
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "Error al enlazar shader program:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::compileShader(unsigned int type, const char* source)
{
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
