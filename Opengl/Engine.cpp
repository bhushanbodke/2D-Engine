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
    Points = new float[Src_width*Src_height * 6];

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
    return true;
}


bool Engine::Init()
{
    // ---------------------------------- Quad and Textures---------------------- 
    GLfloat vertices[] = {
        // Positions        // textcoordes 
    0.0f , 0.0f ,0.0f , 0.0f , 1.0f,
    0.0f , 100.0f ,0.0f , 0.0f , 0.0f,
    100.0f , 100.0f ,0.0f , 1.0f , 0.0f,
    100.0f , 0.0f ,0.0f , 1.0f , 1.0f,
    };
    uint32_t indices[] =
    {
        0,1,2,
        2,3,0,
    };
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glGenBuffers(1, &vbo));
    GLCALL(glGenBuffers(1, &ebo));

    GLCALL(glBindVertexArray(vao));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0));
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT))));
    GLCALL(glEnableVertexAttribArray(1));

    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    //--------------------------------Quad End ---------------------------
    //--------------------------------Circle Begin -----------------------
    int incrementAngle = 10.0f;
    const int triangleCount = 360 / incrementAngle;
    glm::vec2 Center(0, 0);
    float radius = 100;
    std::vector<GLfloat> cvertices(triangleCount * 3);
    float angle = 0.0f;
    cvertices[0] = Center.x;
    cvertices[1] = Center.y;
    cvertices[2] = Center.x + cos(glm::radians(angle)) * radius;
    cvertices[3] = Center.y + sin(glm::radians(angle)) * radius;

    for (int i = 4; i < triangleCount * 3; i += 2)
    {
        angle += incrementAngle;
        cvertices[i + 0] = Center.x + cos(glm::radians(angle)) * radius;
        cvertices[i + 1] = Center.y + sin(glm::radians(angle)) * radius;
    }

    std::vector<uint32_t> cindices((triangleCount + 1) * 3);
    int offset = 0;

    for (int j = 0; j < (triangleCount + 1) * 3; j += 3)
    {
        cindices[j + 0] = 0;
        cindices[j + 1] = 1 + offset;
        cindices[j + 2] = 2 + offset;
        offset++;
    }
    cindicesSize = cindices.size();
    GLCALL(glGenVertexArrays(1, &cvao));
    GLCALL(glGenBuffers(1, &cvbo));

    GLCALL(glBindVertexArray(cvao));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, cvbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * cvertices.size(), &cvertices[0], GL_STATIC_DRAW));

    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), 0));
    GLCALL(glEnableVertexAttribArray(0));

    GLCALL(glGenBuffers(1, &cebo));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cebo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * cindices.size(), &cindices[0], GL_STATIC_DRAW));

    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    //--------------------------------Circle End ---------------------------
    //-------------------------------Points ---------------------------
    GLCALL(glGenVertexArrays(1, &pvao));
    GLCALL(glGenBuffers(1, &pvbo));

    GLCALL(glBindVertexArray(pvao));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, pvbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Src_width*Src_height*6, nullptr, GL_STATIC_DRAW));

    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0));
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))));

    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    //--------------------------------Points End --------------------------

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

void Engine::BackGroundColor(glm::vec4 Col)
{
    GLCALL(glClearColor(Col.r, Col.g, Col.b, Col.a));
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
    delete[] Points;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glDeleteVertexArrays(1, &cvao);
    glDeleteBuffers(1, &cvbo);
    glDeleteBuffers(1, &cebo);

    glDeleteVertexArrays(1, &pvao);
    glDeleteBuffers(1, &pvbo);

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
            std::string t = Title+ "     "+std::to_string(Src_width) + 'x' + std::to_string(Src_height) + "-FPS : " + std::to_string((int)(frames / time));
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

        DrawPixels();
        arrayPos = 0;
        no_points = 0;
        dataSet = 0;
        GLCALL(glfwSwapBuffers(window));
        GLCALL(glfwPollEvents());
    }
}
void Engine::DrawSprite(Sprite& sprite, glm::vec2 Pos, glm::vec2 Size, float angle, glm::vec4 tint)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);

    float sizeX = Size.x / 100;
    float sizeY = Size.y / 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX,sizeY, 1.0f));
    glm::vec2 Center(50.0f, 50.0f);

    model = glm::translate(model, glm::vec3(Pos.x, Pos.y, 0.0f));
    model = glm::translate(model, glm::vec3(Center.x, Center.y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-Center.x, -Center.y, 0.0f));

    Default_Tex_Shader.Activate();
    Default_Tex_Shader.SetUniformMat4("view", view);
    Default_Tex_Shader.SetUniformMat4("model", model);
    

    sprite.bind();
    Default_Tex_Shader.SetUniform1i("m_Texture", sprite.slot);
    Default_Tex_Shader.SetUniform4f("Color", tint.r, tint.g, tint.b, tint.a);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    sprite.unbind();
    Default_Tex_Shader.Deactivate();
}

void Engine::DrawRect(glm::vec2 Pos, glm::vec2 Size, float angle,glm::vec4 Color)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, 0.0f, (float)Src_height, -1.0f, 100.0f);

    float sizeX = Size.x / 100;
    float sizeY = Size.y / 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX, sizeY, 1.0f));
    glm::vec2 Center(50.0f, 50.0f);

    model = glm::translate(model, glm::vec3(Pos.x, Pos.y, 0.0f));
    model = glm::translate(model, glm::vec3(Center.x, Center.y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-Center.x, -Center.y, 0.0f));


    Default_Shader.Activate();
    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r , Color.g, Color.b ,Color.a);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    Default_Shader.Deactivate();

}
void Engine::DrawCircle(glm::vec2 Center, float radius, glm::vec4 Color)
{
    
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);
    
    glm::mat4 model(1.0f);
    float scale = radius / 100;
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
    model = glm::translate(model, glm::vec3(Center.x, Center.y, 0.0f));

    Default_Shader.Activate();
    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);
    glBindVertexArray(cvao);
    glDrawElements(GL_TRIANGLES, cindicesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    Default_Shader.Deactivate();
}

void Engine::DrawPixels()
{
    if (no_points == 0){  }
    else
    {
        if (dataSend != dataSet)
        {
        glBindBuffer(GL_ARRAY_BUFFER, pvbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(GLfloat) * Src_width * Src_height * 6, Points);
        dataSend = dataSet;
        }
        glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);
        glm::mat4 model(1.0f);
        Default_Shader.Activate();
        Default_Shader.SetUniformMat4("view", view);
        Default_Shader.SetUniformMat4("model", model);
        glBindVertexArray(pvao);
        glDrawArrays(GL_POINTS, 0, no_points);
        Default_Shader.Deactivate();
        glBindVertexArray(0);
    }
    
};
void Engine::SetPixel(int x, int y, glm::vec4 Color)
{
    Points[arrayPos + 0] = x;
    Points[arrayPos + 1] = y;
    Points[arrayPos + 2] = Color.r;
    Points[arrayPos + 3] = Color.g;
    Points[arrayPos + 4] = Color.b;
    Points[arrayPos + 5] = Color.a;
    arrayPos += 6;
    no_points += 1;
    dataSet += 1;
};

void Engine::DrawLine(glm::vec2 Pos, float length, float angle, glm::vec4 Color)
{
    float x = Pos.x + cos(glm::radians(angle)) * length;
    float y = Pos.y + sin(glm::radians(angle)) * length;
    GLfloat vertices[] = {
     Pos.x , Pos.y  , 0.0f,
     x ,  y, 0.0f
    };
    GLuint lvbo;
    glGenBuffers(1, &lvbo);
    glBindBuffer(GL_ARRAY_BUFFER, lvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(0);

    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, 0.0f, (float)Src_height, -1.0f, 100.0f);
    glm::mat4 model(1.0f);


    Default_Shader.Activate();
    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);

    glDrawArrays(GL_LINES, 0, 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Default_Shader.Deactivate();
}
