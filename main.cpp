// main.cpp
// ========
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <GLUT/glut.h>
#include "convolution.h"
#include "Timer.h"
#include "BuildKernel.h"

// image data for openGL
struct ImageData{
    GLint   x;      // resolution X
    GLint   y;      // resolution Y
    GLenum  format; // data format (RGB or INDEX..)
    GLenum  type;   // data type (8bit, 16bit or 32bit..)
    GLvoid  *buf;   // image pixel bits
};

// GLUT CALLBACK functions
void displayCB();
void displaySubWin1CB();
void displaySubWin2CB();
void displaySubWin3CB();
void reshapeCB(int w, int h);
void reshapeSubWin1CB(int w, int h);
void reshapeSubWin2CB(int w, int h);
void reshapeSubWin3CB(int w, int h);
void keyboardHandlerCB(unsigned char key, int x, int y);

bool initSharedMem();
void clearSharedMem();
bool loadRawImage(char *fileName, int x, int y, unsigned char *data);

// constants ////////////////////////
const char *FILE_NAME = "lena.raw";
const int  IMAGE_X = 256;
const int  IMAGE_Y = 256;
const int  MAX_NAME = 1024;

// global variables ////////////////
ImageData       *image;
unsigned char   *inBuf;
unsigned char   *outBuf1;
char            fileName[MAX_NAME];
int             imageX;
int             imageY;
int             fontWidth = 8;
int             fontHeight = 13;
int             mainWin, subWin1, subWin2, subWin3, subWin4;
double          time1;



///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // use default image file if not specified
    if(argc == 4)
    {
        strcpy(fileName, argv[1]);
        imageX = atoi(argv[2]);
        imageY = atoi(argv[3]);
    }
    else{
        printf("Usage: %s <image-file> <width> <height>\n", argv[0]);
        strcpy(fileName, FILE_NAME);
        imageX = IMAGE_X;
        imageY = IMAGE_Y;
        printf("\nUse default image \"%s\", (%d,%d)\n", fileName, imageX, imageY);
    }

    // allocate memory for global variables
    if(!initSharedMem()) return 0;

    // open raw image file
    if(!loadRawImage(fileName, imageX, imageY, inBuf))
    {
        clearSharedMem();                       // exit program if failed to load image
        return 0;
    }
    
    BuildKernel();
    
    // define 5x5 Gaussian kernel
    float kernel[25] = { 1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f,
                         4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
                         6/256.0f, 24/256.0f, 36/256.0f, 24/256.0f, 6/256.0f,
                         4/256.0f, 16/256.0f, 24/256.0f, 16/256.0f, 4/256.0f,
                         1/256.0f,  4/256.0f,  6/256.0f,  4/256.0f, 1/256.0f };


    // perform convolution
    Timer t;
    t.start();
    convolve2D(inBuf, outBuf1, imageX, imageY, kernel, 5, 5);
    t.stop();
    time1 = t.getElapsedTimeInMilliSec();
    printf("Normal Convolution: %f ms\n", time1);

//    // print the pixel inputs and outputs.
//    for(int i=0; i < imageX*imageY; ++i)
//    {
//        printf(" %d, in:%d out1:%d\n", i, inBuf[i], outBuf1[i]);
//    }

    return 0;
}



///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    image = new ImageData;
    if(!image)
    {
        printf("ERROR: Memory Allocation Failed.\n");
        return false;
    }

    // allocate input/output buffer
    inBuf = new unsigned char[imageX * imageY];
    outBuf1 = new unsigned char[imageX * imageY];

    if(!inBuf || !outBuf1)
    {
        printf("ERROR: Memory Allocation Failed.\n");
        return false;
    }

    // set image data
    image->x = imageX;
    image->y = imageY;
    image->format = GL_LUMINANCE;
    image->type = GL_UNSIGNED_BYTE;
    image->buf = (GLvoid*)inBuf;

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// clean up shared memory
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
    delete image;
    delete [] inBuf;
    delete [] outBuf1;
}

///////////////////////////////////////////////////////////////////////////////
// load 8-bit RAW image
///////////////////////////////////////////////////////////////////////////////
bool loadRawImage(char *fileName, int x, int y, unsigned char *data)
{
    // check params
    if(!fileName || !data)
        return false;

    FILE *fp;
    if((fp = fopen(fileName, "r")) == NULL)
    {
        printf("Cannot open %s.\n", fileName);
        return false;
    }

    // read pixel data
    fread(data, 1, x*y, fp);
    fclose(fp);

    return true;
}
