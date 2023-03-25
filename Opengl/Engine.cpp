#include "Engine.h"
#include <array>

/*+---------------------------------------------------------------------+
* |                      Error Handling Code                            |
* |                                                                     |
* +---------------------------------------------------------------------+
*/

#define ASSERT(x) assert(x);
#define GLCALL(x) glClearError();\
    x;\
    ASSERT(glCheckError(#x, __FILE__, __LINE__))


static void glClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool glCheckError(const char* functionName, const char* file, int line)
{
    while (GLenum err = glGetError())
    {
        std::cerr << "OpenGL ErrorNo: " << std::hex << err << " FileName: " << file << " || "
            << functionName << " Line: " << line << std::endl;
        return false;
    }
    return true;
}

//Glboal var

Shader Engine::Default_Tex_Shader;
Shader Engine::Default_Shader;

bool Engine::CreateWindow(short width, short height, std::string title,  bool CapFps60)
{
    Src_width = width;
    Src_height = height;
    Title = title;
    glfwInit();
    window = glfwCreateWindow(Src_width, Src_height, Title.c_str(), NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);
    if (CapFps60)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    gladLoadGL();
    glViewport(0, 0, Src_width, Src_height);
    Default_Tex_Shader = Shader("shaders/default.vert", "shaders/texture.frag");
    Default_Shader = Shader("shaders/default.vert", "shaders/default.frag");
    SetBlend(true);
    //v = vertices.data();
    return true;
}

//Sprite* Engine::CreateSprite(std::string filePath, glm::vec2 Pos, glm::vec2 size)
//{
//    Sprite* sprite  =  new Sprite(filePath);
//    Cleanup.push_back(sprite);
//    v = GenQuad(v, Pos.x, Pos.y, size);
//    indexcount += 6;
//    return sprite;
//}
//
//Vertex* Engine::GenQuad(Vertex* v, int x, int y, glm::vec2 size)
//{
//    //bottom left
//    v->Pos.x = x;
//    v->Pos.y = y;
//    v->Pos.z = 0.0f;
//    v->TexCoord.x = 0.0f;
//    v->TexCoord.y = 0.0f;
//    v++;
//
//    //top left
//    v->Pos.x = x;
//    v->Pos.y = y + size.y;
//    v->Pos.z = 0.0f;
//    v->TexCoord.x = 0.0f;
//    v->TexCoord.y = 1.0f;
//    v++;
//
//    //top right
//    v->Pos.x = x + size.x;
//    v->Pos.y = y + size.y;
//    v->Pos.z = 0.0f;
//    v->TexCoord.x = 1.0f;
//    v->TexCoord.y = 1.0f;
//    v++;
//    //bottom right
//    v->Pos.x = x +size.x;
//    v->Pos.y = y;
//    v->Pos.z = 0.0f;
//    v->TexCoord.x = 1.0f;
//    v->TexCoord.y = 0.0f;
//    v++;
//    return v;
//}

bool Engine::Init()
{

    GLfloat vertices[] = {
        // Positions        // textcoordes 
    0.0f , 0.0f ,0.0f , 0.0f , 0.0f,
    0.0f , 100.0f ,0.0f , 0.0f , 1.0f,
    100.0f , 100.0f ,0.0f , 1.0f , 1.0f,
    100.0f , 0.0f ,0.0f , 1.0f , 0.0f,
    };
    uint32_t indices[] =
    {
        0,1,2,
        2,3,0,
    };
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Events();
    CreateAssets();
    return true;
}

void Engine::Events() {
    glfwSetKeyCallback(window, KeyBoard::KeyCallback);
    glfwSetCursorPosCallback(window, Mouse::MouseCallback);
    glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::MouseWheelCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

void Engine::ClearScreen()
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Engine::BackGroundColor(float r, float g, float b, float a)
{
    GLCALL(glClearColor(r,g,b,a));
}

void Engine::resizeCallback(GLFWwindow* window, int width, int height)
{
    GLCALL(glViewport(0, 0, width, height));
}

bool Engine::OnUserCreate()
{
    return false;
}

bool Engine::Render()
{
    return false;
}

void Engine::InputHandling(float deltatime)
{
}

void Engine::CreateAssets()
{
}

Engine::~Engine()
{
    /*for (auto& s : Cleanup)
        delete s;*/
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwDestroyWindow(window);
    glDeleteShader(Default_Tex_Shader.ID);
    glDeleteShader(Default_Shader.ID);
    glfwTerminate();
}

bool Engine::SetBlend(bool blend)
{
    if (blend)
    {
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }
    else
    {
        GLCALL(glDisable(GL_BLEND));
    }
    return true;
}

void Engine::Stop()
{
    glfwSetWindowShouldClose(window, true);
}

void Engine::Run()
{
    float t1 = glfwGetTime();
    float t2 = glfwGetTime();
    float t3 = glfwGetTime();
    int frames = 0.0f;
    float time = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        t2  = glfwGetTime();

        float deltaTime = t2 - t1;
        t1 = t2; 
       
        time += deltaTime;
        if (time >= 1.0f)
        {
            std::string t = Title + "      FPS : " + std::to_string((int)(frames / time));
            glfwSetWindowTitle(window,t.c_str());
            frames = 0.0f;
            time = 0.0f;
        }
        frames++;

        ClearScreen();

        if (!OnUserCreate())
            glfwSetWindowShouldClose(window, true);


        InputHandling(deltaTime);

        if (!Render())
            glfwSetWindowShouldClose(window, true);

        
        GLCALL(glfwSwapBuffers(window));
        GLCALL(glfwPollEvents());
    }
}
void Engine::DrawSprite(Sprite& sprite, glm::vec2 Pos, glm::vec2 Size, glm::vec2 axis, float angle, glm::vec4 tint)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, 0.0f, (float)Src_height, -1.0f, 100.0f);

    float sizeX = Size.x / 100;
    float sizeY = Size.y / 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX,sizeY, 1.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(axis.x, axis.y, 0.0f));
    model = glm::translate(model, glm::vec3(Pos.x, Pos.y, 0.0f));

    Default_Tex_Shader.Activate();
    Default_Tex_Shader.SetUniformMat4("view", view);
    Default_Tex_Shader.SetUniformMat4("model", model);
    

    sprite.bind();
    Default_Tex_Shader.SetUniform1i("m_Texture", sprite.slot);
    Default_Tex_Shader.SetUniform4f("Color", tint.r, tint.g, tint.b, tint.a);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    sprite.unbind();
    Default_Tex_Shader.Deactivate();


}

void Engine::DrawRect(glm::vec2 Pos, glm::vec2 Size, glm::vec2 axis, float angle,glm::vec4 Color)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, 0.0f, (float)Src_height, -1.0f, 100.0f);

    float sizeX = Size.x / 100;
    float sizeY = Size.y / 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX, sizeY, 1.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(axis.x, axis.y, 0.0f));
    model = glm::translate(model, glm::vec3(Pos.x, Pos.y, 0.0f));

    Default_Shader.Activate();
    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r , Color.g, Color.b ,Color.a);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    Default_Shader.Deactivate();

}