#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/fwd.hpp>
class Shader
{
public :
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	//utility uniform fuctions
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setVec3(const std::string& name, const glm::vec3& value)const;
private:
	unsigned int compileShader(unsigned int type, const char* source);
};

#endif