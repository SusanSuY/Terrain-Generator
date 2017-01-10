/*
* Name: Susan Yuen
* MacID: yuens2
* Student ID: 001416198
*/

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <utility>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <climits>

using namespace std;

class Terrain
{
public:
    Terrain();                                      // constructor
    int getSize();                                  // gets terrain size
    void increaseSize();                            // increases terrain size by 50 (if possible)
    void decreaseSize();                            // decreases terrain size by 50 (if possible)
    float getScale(float height);                   // gets color for grayscale
    void drawTerrain(bool wireFrameOverlay, bool quadStrips);   // draws terrain
    void circleAlgorithm();                         // circle algorithm for calculating height
    void calculateNormals();                        // calculates normals for lighting
    void reconstructTerrain(int heightAlgorithm);   // redraws new terrain
    void draw2DTerrain();                           // used to draw 2D representation of terrain (**EXTRA FEATURE)
    void faultAlgorithm();                          // uses fault algorithm to determine heights (**EXTRA FEATURE)
    void particleDeposition();                      // uses particle deposition to determine heights (**EXTRA FEATURE)
    void calculateMinMaxHeight();                   // sets max and min values

private:
    int currentSize;    // current size of terrain
    float maxHeight;    // maximum height
    float minHeight;    // minimum height
    static const int MINIMUM_SIZE = 50;                         // minimum size of terrain
    static const int MAXIMUM_SIZE = 300;                        // maximum size of terrain
    static const int SIZE_STEP = 50;                            // step of terrain size changes
    float heightMap[MAXIMUM_SIZE][MAXIMUM_SIZE];                // height map of vertices
    float normalVectors[MAXIMUM_SIZE][MAXIMUM_SIZE][3];         // normal vectors of vertices
    void resetHeightMap();                                      // resets the height map to 0
    vector<float> getNormalVector(float v1[3], float v2[3]);    // returns the normal vector of two vectors
};

#endif
