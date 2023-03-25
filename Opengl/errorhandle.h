#pragma once
#include "glad.h"
#include <cassert>
#include <iostream>
#include <cerrno>

#define ASSERT(x) assert(x);
#define GLCALL(x) glClearError();\
    x;\
    ASSERT(glCheckError(#x, __FILE__, __LINE__))


static void glClearError()
{
    while(glGetError() != GL_NO_ERROR);
}
static bool glCheckError(const char* functionName , const char* file , int line)
{
    while(GLenum err = glGetError())
    {
        std::cerr << "OpenGL ErrorNo: " <<std::hex<< err <<" FileName: "<< file <<" || " 
        <<functionName<<" Line: "<<line<< std::endl;
        return false;
    }
    return true;
}