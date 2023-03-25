#pragma once

#include "glad.h"
#include <string>
#include<fstream>
#include <iostream>
#include <sstream>
#include <cerrno>
#include <glm/glm.hpp>

std::string get_file_contents(const char* filepath);

class Shader
{
    public:
    GLuint ID; //Shader Program 
    bool is_active = false;
    Shader();

    Shader(const char* vertexFile, const char* fragmentFile);

    void SetUniform1i(const std::string& name , int i1);
    void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
    void SetUniformMat4(const std::string& name , glm::mat4& matrix);

    void Activate();
    void Deactivate();
    void Delete();
};
