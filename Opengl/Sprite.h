#pragma once
#include<string>
#include "glad.h"
#include "stb_Image.h"
class Sprite
{
    private:
        static int currentID;
        unsigned int  m_SpriteID;
        std::string m_filepath;
        unsigned char* SpriteData;

        int m_width , m_height , m_BPP;

    public:

        unsigned int  slot;

        Sprite(const std::string filepath);
        Sprite();

        void bind();
        void unbind();
        void Delete();

        inline int getWidth() {return m_width;};
        inline int getHeight() {return m_height;};
};