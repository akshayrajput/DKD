#define TEXTURE_LOAD_ERROR 0
#include "Texture.h"

using namespace std;

//this function is created to support loading of NPOT png files the code didnt 
GLuint powerOfTwo( GLuint num ) 
{ 
	if( num != 0 ) 
	{ 
		num--; 
		num |= (num >> 1); //Or first 2 bits 
		num |= (num >> 2); //Or next 2 bits 
		num |= (num >> 4); //Or next 4 bits 
		num |= (num >> 8); //Or next 8 bits 
		num |= (num >> 16); //Or next 16 bits 
		num++; 
	} 
  return num; 
}

//load texture from png file and return id value, 0 if error occurs
 GLuint loadTexture(const string filename, int &width, int &height,int &imgheight,int &imgwidth) 
{
   //header for testing if it is a png
   png_byte header[8];
 
   //open file as binary
   FILE *fp = fopen(filename.c_str(), "rb");
   if (!fp) {
     return TEXTURE_LOAD_ERROR;
   }
 
   //read the header
   fread(header, 1, 8, fp);
 
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
 
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //init png reading
   png_init_io(png_ptr, fp);
 
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
 
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
 
   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
 
   //update width and height based on png info
   imgwidth = twidth;
   imgheight = theight;
   width = powerOfTwo(twidth);
   height = powerOfTwo(theight);
 
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   //for 4 byte alignment
   rowbytes += 3 - ((rowbytes-1) % 4); 
    
   // Allocate the image_data as a big block, to be given to opengl
   png_byte *image_data = new png_byte[rowbytes * height];

   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
 
   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     delete[] image_data;
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //Now generate the OpenGL texture object
   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, (GLuint*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   delete[] image_data;
   delete[] row_pointers;
   fclose(fp);
 
   return texture;
 }


Texture::Texture()
{
    //Initialize texture ID
    mTextureID = 0;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;

    //Initialize image dimensions
    mImgHeight = 0;
    mImgWidth = 0;
}

Texture::~Texture()
{
    //Free texture data if needed
    freeTexture();
}

bool Texture::loadTextureFromFile( std::string path )
{
    //Texture loading success
    bool textureLoaded = false;

    GLuint texture;
    int w= 0,h = 0,iw=0,ih=0;
    texture = loadTexture(path,w,h,ih,iw);
    if(texture == 0)
    {
	    printf("Error in loading texture!!\n");
	    textureLoaded = false;
    }

    else
   {
	   mImgWidth = iw;
	   mImgHeight = ih;

	   mTextureWidth = w;
	   mTextureHeight = h;

	   mTextureID = texture;
	   textureLoaded = true;
   }
 
    //Report error
    if( !textureLoaded )
    {
         printf( "Unable to load %s\n", path.c_str());
    }

    return textureLoaded;
}



void Texture::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
    mImgHeight = 0;
    mImgWidth = 0;
}

void Texture::render(GLfloat x,GLfloat y )
{
    //If the texture exists
    if( mTextureID != 0 )
    {
         //Remove any previous transformations
        glLoadIdentity();

        //Move to rendering point
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Render textured quad
        glBegin( GL_QUADS );
	    
	    
	    /*glTexCoord2f( 0.f, 0.f ); glVertex2f( 0.f, 0.f ); glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth, 0.f ); glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth, mTextureHeight ); glTexCoord2f( 0.f, 1.f ); glVertex2f( 0.f, mTextureHeight );*/
	    
            glTexCoord2f( 0.f, 0.f ); glVertex2f( 0.f, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f( 0.f, 0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth, 0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth, mTextureHeight );
	    
        glEnd();
    }
}

GLuint Texture::getTextureID()
{
    return mTextureID;
}

GLuint Texture::textureWidth()
{
    return mTextureWidth;
}

GLuint Texture::textureHeight()
{
    return mTextureHeight;
}

