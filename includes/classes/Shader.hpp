#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
    public:
    unsigned int ID; // the program ID
  
    Shader(const char* vertexPath, const char* fragmentPath); // constructor reads and builds the shader
    void Use();// use/activate the shader

    void SetBool(const std::string &name, bool value) const;  
    void SetInt(const std::string &name, int value) const;   
    void SetFloat(const std::string &name, float value) const;
    void SetFloat4(const std::string &name, float value1, float value2, float value3, float value4) const;
    void Setmat4(const std::string &name, glm::mat4 value) const;

    private:
        void CheckCompileErrors(unsigned int shader, std::string type);
};
  
#endif