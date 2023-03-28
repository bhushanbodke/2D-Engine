#include <ft2build.h>
#include FT_FREETYPE_H  
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
    // -------------------------------Text render begin ----------------
    glGenVertexArrays(1, &tvao);
    glGenBuffers(1, &tvbo);

    glBindVertexArray(tvao);
    glBindBuffer(GL_ARRAY_BUFFER, tvbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // -------------------------------Text render end ----------------

    Events();
    TextInit();
    CreateAssets();
    return true;
}
void Engine::TextInit()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    FT_Face face;
    if (FT_New_Face(ft, "fonts/segoeui.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (uint32_t c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
            continue;
        }
        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
        };
        Characters[c] = character;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Engine::RenderText(std::string text, float x, float y, float scale, glm::vec4 Color)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, 0.0f, (float)Src_height, -1.0f, 100.0f);

    glm::mat4 model(1.0f);
    y = Src_height - y;
    
    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);

    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);
    Default_Shader.SetUniform1i("text", mode::text);
    Default_Shader.SetUniform1i("m_Texture",0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(tvao);
    for (auto& c : text)
    {
        Character ch = Characters[c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, tvbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;

    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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

bool Engine::Render(int fps)
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
    glDeleteShader(Default_Shader.ID);
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
    int fps = 0;
    Default_Shader.Activate();
    while (!glfwWindowShouldClose(window))
    {
        t2  = glfwGetTime();

        float deltaTime = t2 - t1;
        t1 = t2; 
       
        time += deltaTime;
        if (time >= 1.0f)
        {
            fps = (int)(frames / time);
            std::string t = Title+ "   -FPS : " + std::to_string((int)(frames / time));
            glfwSetWindowTitle(window,t.c_str());
            frames = 0.0f;
            time = 0.0f;
        }
        frames++;

        ClearScreen();

        if (!OnUserCreate())
            glfwSetWindowShouldClose(window, true);


        InputHandling(deltaTime);

        if (!Render(fps))
            glfwSetWindowShouldClose(window, true);

        RenderPixels();
        arrayPos = 0;
        no_points = 0;
        dataSet = 0;
        GLCALL(glfwSwapBuffers(window));
        GLCALL(glfwPollEvents());
    }
    Default_Shader.Deactivate();
}
void Engine::DrawSprite(Sprite& sprite, uint16_t x, uint16_t y, uint16_t width, uint16_t height, glm::vec4 tint, float angle)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);

    float sizeX = (float)width / 100;
    float sizeY = (float)height/ 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX,sizeY, 1.0f));
    glm::vec2 Center(50.0f, 50.0f);

    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::translate(model, glm::vec3(Center.x, Center.y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-Center.x, -Center.y, 0.0f));

    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    

    sprite.bind();
    Default_Shader.SetUniform1i("m_Texture", sprite.slot);
    Default_Shader.SetUniform1i("text",mode::texture);
    Default_Shader.SetUniform4f("Color", tint.r, tint.g, tint.b, tint.a);

    GLCALL(glBindVertexArray(vao));
    GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
    sprite.unbind();
}

void Engine::DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,glm::vec4 Color, float angle)
{
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height,0.0f, -1.0f, 100.0f);

    float sizeX = (float)width / 100;
    float sizeY = (float)height / 100;

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(sizeX, sizeY, 1.0f));
    glm::vec2 Center(50.0f, 50.0f);

    model = glm::translate(model, glm::vec3( x, y, 0.0f));
    model = glm::translate(model, glm::vec3(Center.x, Center.y, 0.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-Center.x, -Center.y, 0.0f));


    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform1i("text", mode::solid);
    Default_Shader.SetUniform4f("Color", Color.r , Color.g, Color.b ,Color.a);

    GLCALL(glBindVertexArray(vao));
    GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));

}
void Engine::DrawCircle(uint16_t x, uint16_t y, float radius, glm::vec4 Color)
{
    
    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);
    
    glm::mat4 model(1.0f);
    float scale = radius / 100;
    model = glm::translate(model, glm::vec3(x , y , 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);
    Default_Shader.SetUniform1i("text", mode::solid);

    GLCALL(glBindVertexArray(cvao));
    GLCALL(glDrawElements(GL_TRIANGLES, cindicesSize, GL_UNSIGNED_INT, 0));
    GLCALL(glBindVertexArray(0));
}

void Engine::RenderPixels()
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
        Default_Shader.SetUniformMat4("view", view);
        Default_Shader.SetUniformMat4("model", model);
        Default_Shader.SetUniform1i("text", mode::Point);
        GLCALL(glBindVertexArray(pvao));
        GLCALL(glDrawArrays(GL_POINTS, 0, no_points));
        GLCALL(glBindVertexArray(0));
    }
    
};
void Engine::DrawPixel(int x, int y, glm::vec4 Color)
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

void Engine::DrawLine(uint16_t x, uint16_t y ,float length, glm::vec4 Color, float angle)
{
    float x1 = x + cos(glm::radians(angle)) * length;
    float y1 = y + sin(glm::radians(angle)) * length;
    GLfloat vertices[] = {
     x ,  y  , 0.0f,
     x1 ,  y1, 0.0f
    };
    GLuint lvbo;
    GLCALL(glGenBuffers(1, &lvbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, lvbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0));
    GLCALL(glEnableVertexAttribArray(0));

    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);
    glm::mat4 model(1.0f);


    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);

    GLCALL(glDrawArrays(GL_LINES, 0, 2));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
void Engine::DrawLine(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, glm::vec4 Color)
{
    GLfloat vertices[] = {
     x ,  y  , 0.0f,
     x1 ,  y1, 0.0f
    };
    GLuint lvbo;
    GLCALL(glGenBuffers(1, &lvbo));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, lvbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0));
    GLCALL(glEnableVertexAttribArray(0));

    glm::mat4 view = glm::ortho(0.0f, (float)Src_width, (float)Src_height, 0.0f, -1.0f, 100.0f);
    glm::mat4 model(1.0f);


    Default_Shader.SetUniformMat4("view", view);
    Default_Shader.SetUniformMat4("model", model);
    Default_Shader.SetUniform4f("Color", Color.r, Color.g, Color.b, Color.a);

    GLCALL(glDrawArrays(GL_LINES, 0, 2));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
