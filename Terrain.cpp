/*
* Name: Susan Yuen
* MacID: yuens2
* Student ID: 001416198
*/

#include "Terrain.h"

using namespace std;

/* constructor for Terrain */
Terrain::Terrain()
{
    currentSize = 100;   // default size upon terrain construction

    // call functions to set up initial terrain
    circleAlgorithm();  // call circle algorithm by default
    calculateNormals(); // calculate normals
    calculateMinMaxHeight();    // sets max and min height values
}

/* returns current size of terrain */
int Terrain::getSize()
{
    return currentSize;
}

/* increases size of terrain, if possible */
void Terrain::increaseSize()
{
    // if current size with size step will not exceed maximum size
    if (currentSize <= MAXIMUM_SIZE - SIZE_STEP)
    {
        currentSize += SIZE_STEP;   // increase terrain by size step
    }
}

/* decrease size of terrain, if possible */
void Terrain::decreaseSize()
{
    // if current size without size step will not exceed minimum size
    if (currentSize >= MINIMUM_SIZE + SIZE_STEP)
    {
        currentSize -= SIZE_STEP;   // decrease terrain by size step
    }
}

float Terrain::getScale(float height)
{
    float range = maxHeight - minHeight;
    cout << "minHeight = " << minHeight << endl;
    cout << "maxHeight = " << maxHeight << endl;
    return ((height - minHeight)/range);
}

/* draws the terrain */
void Terrain::drawTerrain(bool wireFrameOverlay, bool quadStrips)
{
    const float HILL_HEIGHT = currentSize/MINIMUM_SIZE+4;   // multiply hill heights by this factor (get taller hills)

    // draw solid filled quad terrain
    if (quadStrips)
    {
        glBegin(GL_QUADS);
        for (int x=0; x<currentSize-1; x++)
        {
            for (int z=0; z<currentSize-1; z++)
            {
                // draws each tile in the terrain
                glNormal3fv(normalVectors[x][z]);   // sends normals to OpenGL
                glColor3f(getScale(heightMap[x][z]), getScale(heightMap[x][z]), getScale(heightMap[x][z]));   // colours the vertex based on height
                glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                glNormal3fv(normalVectors[x][z+1]);
                glColor3f(getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]));
                glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                glNormal3fv(normalVectors[x+1][z+1]);
                glColor3f(getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]));
                glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                glNormal3fv(normalVectors[x+1][z]);
                glColor3f(getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]));
                glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
            }
        }
        glEnd();

        // if wireframe required as well, draw the wireframe version (quad)
        if (wireFrameOverlay)
        {
            glColor3f(0, 0, 0);         // black coloured lines
            glTranslatef(0, 0.1, 0);    // translate wireframe slightly above for full visibility
            for (int x=0; x<currentSize-1; x++)
            {
                for (int z=0; z<currentSize-1; z++)
                {
                    // draws each tile in the terrain
                    glBegin(GL_LINE_LOOP);
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                    glEnd();
                }
            }
        }
    }

    // draw filled terrain with triangles
    if (!quadStrips)
    {
        for (int x=0; x<currentSize-1; x++)
        {
            for (int z=0; z<currentSize-1; z++)
            {
                // draws each triangle
                glBegin(GL_TRIANGLES);
                if (x%2==0) // if x is even...
                {
                    // bottom left triangle
                    glNormal3fv(normalVectors[x][z]);
                    glColor3f(getScale(heightMap[x][z]), getScale(heightMap[x][z]), getScale(heightMap[x][z]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                    glNormal3fv(normalVectors[x][z+1]);
                    glColor3f(getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                    glNormal3fv(normalVectors[x+1][z]);
                    glColor3f(getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);

                    // top right triangle
                    glNormal3fv(normalVectors[x][z+1]);
                    glColor3f(getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                    glNormal3fv(normalVectors[x+1][z+1]);
                    glColor3f(getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                    glNormal3fv(normalVectors[x+1][z]);
                    glColor3f(getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
                }
                else    // if x is odd...
                {
                    // top left triangle
                    glNormal3fv(normalVectors[x][z+1]);
                    glColor3f(getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]), getScale(heightMap[x][z+1]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                    glNormal3fv(normalVectors[x+1][z+1]);
                    glColor3f(getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                    glNormal3fv(normalVectors[x][z]);
                    glColor3f(getScale(heightMap[x][z]), getScale(heightMap[x][z]), getScale(heightMap[x][z]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);

                    // bottom right triangle
                    glNormal3fv(normalVectors[x+1][z+1]);
                    glColor3f(getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]), getScale(heightMap[x+1][z+1]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                    glNormal3fv(normalVectors[x+1][z]);
                    glColor3f(getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]), getScale(heightMap[x+1][z]));
                    glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
                    glNormal3fv(normalVectors[x][z]);
                    glColor3f(getScale(heightMap[x][z]), getScale(heightMap[x][z]), getScale(heightMap[x][z]));
                    glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                }
                glEnd();
            }
        }

        // if wireframe required as well, draw the wireframe version (triangle)
        if (wireFrameOverlay)
        {
            glColor3f(0, 0, 0);         // black coloured lines
            glTranslatef(0, 0.1, 0);    // translate wireframe slightly above for full visibility
            for (int x=0; x<currentSize-1; x++)
            {
                for (int z=0; z<currentSize-1; z++)
                {
                    // draws each triangle
                    glBegin(GL_LINE_LOOP);
                    if (x%2==0)
                    {
                        // bottom left triangle
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);

                        // top right triangle
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
                    }
                    else
                    {
                        // top left triangle
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z+1], z+1);
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);

                        // bottom right triangle
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z+1], z+1);
                        glVertex3f(x+1, HILL_HEIGHT*heightMap[x+1][z], z);
                        glVertex3f(x, HILL_HEIGHT*heightMap[x][z], z);
                    }
                    glEnd();
                }
            }
        }
    }
}

/* circle algorithm to randomly set hills in terrain */
void Terrain::circleAlgorithm()
{
    for (int i=0; i<currentSize/4; i++)     // iterates up to currentSize/4
    {
        // randomly select point to be centre of circle
        int centerX = rand()%currentSize;
        int centerZ = rand()%currentSize;
        int radius = (rand()%((currentSize/10)*3))+currentSize/10+1;   // random int between currentSize/10 to (currentSize/10)*3
        float disp = 1.0;   // maximum height variation

        for (int x=0; x<currentSize; x++)
        {
            for (int z=0; z<currentSize; z++)
            {
                int dx = x - centerX;
                int dz = z - centerZ;
                float distance = sqrtf((dx*dx) + (dz*dz));  // distance between point and center of circle
                float pd = (distance*2)/radius;
                if (fabs(pd) <= 1.0)
                {
                    heightMap[x][z] += (disp/2.0)+(cos(pd*3.14)*(disp/2.0));    // set height
                }
            }
        }
    }
}

/* calculates vertex normals */
void Terrain::calculateNormals()
{
    float v1[3];        // up vector
    float v2[3];        // down vector
    float v3[3];        // right vector
    float v4[3];        // left vector

    vector<float> n;    // temporarily holds normal vector
    float n1[3];        // normal of top right quad
    float n2[3];        // normal of top left quad
    float n3[3];        // normal of bottom right quad
    float n4[3];        // normal of bottom left quad

    // set up unchanging vector values
    v1[0] = 0;  // up vector
    v1[2] = 1;
    v2[0] = 0;  // down vector
    v2[2] = -1;
    v3[0] = 1;  // right vector
    v3[2] = 0;
    v4[0] = -1; // left vector
    v4[2] = 0;

    // for each vertex...
    for (int x=0; x<currentSize; x++)
    {
        for (int z=0; z<currentSize; z++)
        {
            // set y-values of vectors
            v1[1] = heightMap[x][z+1] - heightMap[x][z];    // up vector
            v2[1] = heightMap[x][z-1] - heightMap[x][z];    // down vector
            v3[1] = heightMap[x+1][z] - heightMap[x][z];    // right vector
            v4[1] = heightMap[x-1][z] - heightMap[x][z];    // left vector

            // normal vector for top right quad
            n = getNormalVector(v3, v1);
            copy(n.begin(), n.end(), n1);       // copies normal vector to n1

            // normal vector for top left quad
            n = getNormalVector(v1, v4);
            copy(n.begin(), n.end(), n2);       // copies normal vector to n2

            // normal vector for bottom right quad
            n = getNormalVector(v2, v3);
            copy(n.begin(), n.end(), n3);       // copies normal vector to n3

            // normal vector for bottom left quad
            n = getNormalVector(v4, v2);
            copy(n.begin(), n.end(), n4);       // copies normal vector to n4

            // set average direction of normal vectors
            normalVectors[x][z][0] = n1[0] + n2[0] + n3[0] + n4[0];
            normalVectors[x][z][1] = n1[1] + n2[1] + n3[1] + n4[1];
            normalVectors[x][z][2] = n1[2] + n2[2] + n3[2] + n4[2];

            // normalize normal vector to unit length
            float magnitude = sqrtf(normalVectors[x][z][0]*normalVectors[x][z][0]
                                    + normalVectors[x][z][1]*normalVectors[x][z][1]
                                    + normalVectors[x][z][2]*normalVectors[x][z][2]);
            normalVectors[x][z][0] /= magnitude;
            normalVectors[x][z][1] /= magnitude;
            normalVectors[x][z][2] /= magnitude;
        }
    }
}

/* returns normal vector given two specified vectors */
vector<float> Terrain::getNormalVector(float v1[3], float v2[3])
{
    vector<float> normal(3);    // normal vector to be returned
    float crossProduct[3];      // cross product of two specified vectors

    // calculate cross product
    crossProduct[0] = v1[1]*v2[2] - v1[2]*v2[1];
    crossProduct[1] = v1[2]*v2[0] - v1[0]*v2[2];
    crossProduct[2] = v1[0]*v2[1] - v1[1]*v2[0];

    // return normal vector
    float magnitude = sqrtf(crossProduct[0]*crossProduct[0]
                            + crossProduct[1]*crossProduct[1]
                            + crossProduct[2]*crossProduct[2]);
    normal[0] = crossProduct[0]/magnitude;
    normal[1] = crossProduct[1]/magnitude;
    normal[2] = crossProduct[2]/magnitude;
    return normal;
}

/* redraws a new terrain */
void Terrain::reconstructTerrain(int heightAlgorithm)
{
    resetHeightMap();   // set height map to 0

    // use height algorithm to determine heights
    if (heightAlgorithm == 1)
    {
        circleAlgorithm();  // uses circle algorithm
    }
    else if (heightAlgorithm == 2)
    {
        faultAlgorithm();   // use fault algorithm
    }
    else
    {
        particleDeposition();   // use particle deposition
    }

    calculateNormals(); // calculate vector normals
    calculateMinMaxHeight();    // sets max and min height values
}

/* draws the 2D representation of the terrain */
void Terrain::draw2DTerrain()
{
    float distanceBetween = 0.005;  // distance between each point

    glPushMatrix();
    glLoadIdentity();
    glScalef(1, -1, 1);     // horizontal flip
    glTranslatef(-distanceBetween*currentSize/2, -distanceBetween*currentSize/2, 0);    // moves map to center of window
    for (int x=0; x<currentSize; x++)
    {
        for (int y=0; y<currentSize; y++)
        {
            glColor3f(heightMap[x][y], heightMap[x][y], heightMap[x][y]);   // colour each point based on height
            glBegin(GL_POINTS);
            glVertex2f(distanceBetween*x, distanceBetween*y);   // draw each point
            glEnd();
        }
    }
    glPopMatrix();
}

/* resets height map and normal vectors to 0 */
void Terrain::resetHeightMap()
{
    // reset height map and normal vectors
    for (int x=0; x<currentSize; x++)
    {
        for (int z=0; z<currentSize; z++)
        {
            heightMap[x][z] = 0;            // reset height map
            normalVectors[x][z][0] = 0;     // reset normals
            normalVectors[x][z][1] = 0;
            normalVectors[x][z][2] = 0;
        }
    }
}

/* fault algorithm to randomly select heights */
void Terrain::faultAlgorithm()
{
    float displacement = 0.1;   // height displacement on each iteration

    for (int i=0; i<3*currentSize; i++)   // iterate i (3*currentSize) times
    {
        // find random line
        float v = static_cast <float>(rand());
        float a = sin(v);
        float b = cos(v);
        float d = sqrtf(2*currentSize*currentSize);
        float c = (static_cast <float>(rand()) / static_cast <float>(RAND_MAX)) * d - d/2;

        // set height
        for (int x=0; x<currentSize; x++)
        {
            for (int z=0; z<currentSize; z++)
            {
                if (a*x + b*z - c > 0)
                {
                    heightMap[x][z] += displacement;
                }
                else
                {
                    heightMap[x][z] -= displacement;
                }
            }
        }
    }
}

/* particle deposition to randomly determine height */
void Terrain::particleDeposition()
{
    // choose random point on terrain
    int x = rand()%currentSize;
    int z = rand()%currentSize;

    for (int i=0; i<500*currentSize; i++)   // iterate i (500*currentSize) times
    {
        heightMap[x][z] += 0.1; // increase height at (x,z)
        int v = rand()%4+1;     // random integer between 1 to 4
        switch(v)
        {
        case 1:
            if (x<currentSize)
            {
                x++;    // move right if possible
            }
            else
            {
                x--;    // else move left
            }
            break;

        case 2:
            if (x>0)
            {
                x--;    // move left if possible
            }
            else
            {
                x++;    // else move right
            }
            break;

        case 3:
            if (z<currentSize)
            {
                z++;    // move closer if possible
            }
            else
            {
                z--;    // else move away
            }
            break;

        case 4:
            if (z>0)
            {
                z--;    // move away if possible
            }
            else
            {
                z++;    // else move closer
            }
            break;
        }
    }
}

void Terrain::calculateMinMaxHeight()
{
    minHeight = INT_MAX;
    maxHeight = 0;

    for (int x=0; x<currentSize; x++)
    {
        for (int z=0; z<currentSize; z++)
        {
            if (heightMap[x][z] < minHeight)
            {
                minHeight = heightMap[x][z];
            }

            if (heightMap[x][z] > maxHeight)
            {
                maxHeight = heightMap[x][z];
            }
        }
    }
}
