#include "Sprite.h"
#include "errorhandle.h"

int Sprite::currentID = 1;

Sprite::Sprite(const std::string filepath)
    :m_filepath(filepath), m_height(0),m_width(0), SpriteData(nullptr), slot(currentID++)
{
    stbi_set_flip_vertically_on_load(1);
    SpriteData = stbi_load(filepath.c_str(),&m_width, &m_height,&m_BPP , 4);
       
    GLCALL(glGenTextures(1 , &m_SpriteID));
    GLCALL(glBindTexture(GL_TEXTURE_2D , m_SpriteID));
    int i;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &i);


    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));   
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,SpriteData));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

    if(SpriteData)
    {
        stbi_image_free(SpriteData);
    };
}

Sprite::Sprite()
{
}

void Sprite::bind()
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_SpriteID));
}

void Sprite::unbind()
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Sprite::Delete()
{
    GLCALL(glDeleteTextures(1, &m_SpriteID));
}
