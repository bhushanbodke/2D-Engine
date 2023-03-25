#include "ShaderClass.h"
#include "errorhandle.h"

std::string get_file_contents(const char* filepath)
{
    std::ifstream file(filepath , std::ios::binary);
    if (file)
    {
        std::string contents;
        file.seekg(0,std::ios::end);
        int size = file.tellg();
        contents.resize(size);
        file.seekg(0,std::ios::beg);
        file.read(&contents[0] , contents.size());
        file.close();

        //std::cout << contents << std::endl;
        return contents;

    }
    throw(errno);
}
Shader::Shader()
{

}
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertShader = get_file_contents(vertexFile);
    std::string fragShader = get_file_contents(fragmentFile);

    const char* vertexShaderSource = vertShader.c_str();
    const char* fragmentShaderSource = fragShader.c_str();

    // shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader , 1 ,&vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint FragementShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragementShader , 1 , &fragmentShaderSource ,NULL);
    glCompileShader(FragementShader);

    // shader program 
    ID = glCreateProgram();
    glAttachShader(ID , vertexShader);
    glAttachShader(ID , FragementShader);
    glLinkProgram(ID);

    glDeleteShader(FragementShader);
    glDeleteShader(vertexShader);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4& matrix)
{
    if (!is_active)
    {
        is_active = true;
        Activate();
    }
    int value = glGetUniformLocation(ID, name.c_str());
    GLCALL(glUniformMatrix4fv(value, 1, GL_FALSE, &matrix[0][0]));
}



void Shader::SetUniform1i(const std::string& name , int i1)
{
    if (!is_active)
    {
        is_active = true;
        Activate();
    }
    int val = glGetUniformLocation(ID, name.c_str());
    GLCALL(glUniform1i(val,i1));
}
void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4)
{
    if (!is_active)
    {
        is_active = true;
        Activate();
    }
    GLCALL(glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4));
}

void Shader::Activate()
{
    if (!is_active)
    {
        glUseProgram(ID);
        is_active = true;
    }
};
void Shader::Deactivate()
{
    if (is_active)
    {
        glUseProgram(0);
        is_active = false;
    }

};
void Shader::Delete()
{
    glDeleteProgram(ID);
}
