#ifndef TEXTURE_H
#define TEXTURE_H

#include "allstdlib.h"

class Texture
{
    public:
        Texture();

        ~Texture();
        bool loadTextureFromFile( std::string path );
        void freeTexture();
        void render( GLfloat x, GLfloat y );
        GLuint getTextureID();
        GLuint textureWidth();
        GLuint textureHeight();

    private:

        //Texture name
        GLuint mTextureID;
        //Texture dimensions
        GLuint mTextureWidth;
        GLuint mTextureHeight;
	//Image Dimensions
	GLuint mImgWidth;
	GLuint mImgHeight;
};

#endif
