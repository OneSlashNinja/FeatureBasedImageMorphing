/******************************************************************************************
The "morph" program is to do the morph
*******************************************************************************************/

#ifndef MORPH_H
#define MORPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>

#include <Magick++.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "geometricTool.h"

using namespace std;
using namespace Magick;

//The default size of the window when there is no picture
const int WIDTH = 600;
const int HEIGHT = 600;

//whether it would display the dest image
bool destFlag = false;

//The width and height of the picture
int imgWidthOrig = 0;
int imgHeightOrig = 0;
int imgWidthDest = 0;
int imgHeightDest = 0;

//the pixel
typedef struct pix{
  unsigned char r,g,b,a;
} pixel;

//The pixmap is the pointer array which contains the pointers that point at the pixels
pixel ** srcImgMap;
pixel ** dstImgMap;
pixel ** morphMap;

//the featureline of sourceImage, destImage and the morphImage
vector<FeatureLine> srcLines;
vector<FeatureLine> dstLines;
vector<FeatureLine> morphLines;

//the GUI variables
int currentButton;
int currentPhase = 0;
Point * hitpoint;
Point * startPoint;

//The name of input and output files
string inputFileOrig;
string inputFileDest;
string outputFile;
//the parameter of the weight
string pStr, aStr, bStr, tStr;
float p = 0;
float a = 1;
float b = 2;
float t = 0.5;

//The significant object from magick for the program this time
Image image;

/********************************************************************************
This function would allocate the memory space for the pixmap
********************************************************************************/
void allocPixmap(int w, int h, pixel ** &map);

/********************************************************************************
This function would read from a image file of various types and store the RGBA 
info into the "pixmap".
********************************************************************************/
//notice that if you donot use the "unsigned char * &map" instead of "unsigned char * map"
//what you did in the function is only to initialize the local pointer to point at the pixmap
void imgRead(string filename, pixel ** &map);


/********************************************************************************
This function would write the image from "pixmap" into a image file.The type
also can be various.
********************************************************************************/
void imgWrite(string filename, pixel ** map);

/********************************************************************************
This function is used to deal with the keyboard event
********************************************************************************/
void handleKey(unsigned char key, int x, int y);

/********************************************************************************
This function is used to deal with the mouse event
********************************************************************************/
void AdjustMouse(int& x, int& y);
void handleMotion(int x, int y);
void srcImgMouseHandle(int button, int state, int x, int y);
void dstImgMouseHandle(int button, int state, int x, int y);

/********************************************************************************
the reshape callback may responds to the user resizing the display window.
********************************************************************************/
void handleReshape(int w, int h);

/********************************************************************************
The display function is used for gult to load.It would 
use the data from to "pixmap" to draw a image. It also help the reshape function.
If the user increases the size of the display window so that it is 
bigger than the image, the image should remain centered 
in the window. If the user decreases the size of the display window so that it is 
smaller than the image the image should be uniformly 
scaled down to the largest size that will still fit the window.
********************************************************************************/
void display(void);

/********************************************************************************
Based on the arguments to initialize the program
********************************************************************************/
void coreInit(int argc, char * argv[]);

/********************************************************************************
The funtion to initialize the glut and display
********************************************************************************/
void windowInit(void);


/********************************************************************************
The core funtions to do the morph
********************************************************************************/
void lineInterpolate(const vector<FeatureLine>& sourceLines, 
                     const vector<FeatureLine>& destLines , vector<FeatureLine>& interLines, float t);
                     
void warp(const Vector2d& interPt, const vector<FeatureLine>& interLines,
          const vector<FeatureLine>& sourceLines,
          float p, float a, float b, Vector2d& src);

// bilinear interpolation
void bilinear(pixel ** &Im, float row, float col, pixel& pix);

void ColorInterPolate(const Vector2d& Src_P, 
                      const Vector2d& Dest_P, 
                      float t, pixel ** &imgSrc,
                      pixel ** &imgDest, pixel& rgb);
                      
void doMorph();

#endif
