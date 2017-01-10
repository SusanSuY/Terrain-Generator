/*
* Name: Susan Yuen
* MacID: yuens2
* Student ID: 001416198
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "Terrain.h"

// windows
int window1;
int window2;

// camera setup
float xAxisRotation = 0;        // rotation around x axis
float yAxisRotation = 0;        // rotation around y axis

// set up lighting
float lightPos1[] = {-400, 100, -250, 1};
float lightPos2[] = {400, 100, -150, 1};
float diffuseLight1[] = {0, 0.2, 1, 1};     // blue light (light source 1)
float diffuseLight2[] = {1, 0.6, 0, 1};     // amber light (light source 2)
float ambientLight[] = {0, 0, 0, 1};

// used to determine visual modes
int wireFrameMode = 1;          // 1 = solid, 2 = both, 3 = wireframe
bool lightingMode = true;       // true = show lighting, false = no lighting
bool flatShadingMode = false;   // true = turn on flat shading, false = turn off flat shading
bool quadStripMode = true;      // true = draws with quads, false = draws with triangles
int heightAlgorithm = 1;        // 1 = circle algorithm, 2 = fault algorithm, 3 = particle deposition

Terrain terrain;        // The terrain

float scaleFactor = 0.5/(terrain.getSize()/50); // scales terrain to fit screen
int oldSize;                                    // used for changes in terrain size

/* handles user keyboard input */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
    case 27:        // 'esc' key
        exit (0);   // quits program
        break;

    // toggles wireframe mode
    case 'w':
        // toggles between the three modes
        if (wireFrameMode < 3)
        {
            wireFrameMode++;
        }
        else
        {
            wireFrameMode = 1;
        }

        // sets the wireframe mode
        if (wireFrameMode == 1 || wireFrameMode == 2)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // solid
        }
        else if (wireFrameMode == 3)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // wireframe
        }
        break;

    // toggles lighting mode
    case 'l':
        lightingMode = !lightingMode;   // toggles mode

        if (lightingMode)
        {
            glEnable(GL_LIGHTING);  // enables lighting mode
        }
        else
        {
            glDisable(GL_LIGHTING); // disables lighting mode
        }
        break;

    // toggles flat shading mode
    case 'f':
        flatShadingMode = !flatShadingMode; // toggles mode

        if (flatShadingMode)
        {
            glShadeModel(GL_FLAT);      // enables flat shading
        }
        else
        {
            glShadeModel(GL_SMOOTH);    // enables smooth shading
        }
        break;

    // resets the terrain
    case 'r':
        terrain.reconstructTerrain(heightAlgorithm);    // redraws random terrain
        glutPostRedisplay();        // shows to main window
        glutSetWindow(window2);     // shows to second window
        break;

    // moves light source 1 in +z direction
    case 'u':
        lightPos1[2]+=2;
        break;

    // moves light source 1 in -z direction
    case 'j':
        lightPos1[2]-=2;
        break;

    // moves light source 1 in +x direction
    case 'k':
        lightPos1[0]+=2;
        break;

    // moves light source 1 in -x direction
    case 'h':
        lightPos1[0]-=2;
        break;

    // moves light source 2 in +z direction
    case 'g':
        lightPos2[2]+=2;
        break;

    // moves light source 2 in -z direction
    case 'b':
        lightPos2[2]-=2;
        break;

    // moves light source 2 in +x direction
    case 'n':
        lightPos2[0]+=2;
        break;

    // moves light source 2 in -x direction
    case 'v':
        lightPos2[0]-=2;
        break;

    // turns on triangle mode
    case 't':
        quadStripMode = false;
        break;

    // turns on quad mode
    case 'y':
        quadStripMode = true;
        break;

    // decreases size of terrain
    case 'o':
        oldSize = terrain.getSize();
        terrain.decreaseSize();     // decreases terrain size
        // only reconstruct terrain if size changed
        if (oldSize != terrain.getSize())
        {
            terrain.reconstructTerrain(heightAlgorithm);
            scaleFactor = 0.5/(terrain.getSize()/50);   // update scaleFactor for camera
            glutPostRedisplay();        // show to main window
            glutSetWindow(window2);     // show to second window
        }
        break;

    // increases size of terrain
    case 'p':
        oldSize = terrain.getSize();
        terrain.increaseSize();     // increases terrain size
        // only reconstruct terrain if size changed
        if (oldSize != terrain.getSize())
        {
            terrain.reconstructTerrain(heightAlgorithm);
            scaleFactor = 0.5/(terrain.getSize()/50);   // update scaleFactor for camera
            glutPostRedisplay();        // show to main window
            glutSetWindow(window2);     // show to second window
        }
        break;

    // toggle between height algorithms
    case 'x':
        if (heightAlgorithm < 3)
        {
            heightAlgorithm++;
        }
        else
        {
            heightAlgorithm = 1;
        }

        // draw terrain with new height algorithm
        terrain.reconstructTerrain(heightAlgorithm);    // redraws terrain
        glutPostRedisplay();        // show to main window
        glutSetWindow(window2);     // show to second window
        break;
    }
    glutPostRedisplay();    // re-displays
}

/* handles special key input */
void special(int key, int x, int y)
{
    /* arrow key presses move the camera */
    switch(key)
    {
    case GLUT_KEY_LEFT:
        yAxisRotation--;    // rotate y-axis in negative direction
        break;

    case GLUT_KEY_RIGHT:
        yAxisRotation++;    // rotate y-axis in positive direction
        break;

    case GLUT_KEY_UP:
        if (xAxisRotation < 45) // prevents terrain from flipping
        {
            xAxisRotation++;    // rotate x-axis in positive direction
        }
        break;

    case GLUT_KEY_DOWN:
        if (xAxisRotation > -45)    // prevents terrain from flipping
        {
            xAxisRotation--;    // rotate x-axis in negative direction
        }
        break;
    }
    glutPostRedisplay();
}

/* sets up OpenGL */
void init(void)
{
    // intially clears colour
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);

    // enables lighting and colour material
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // sets matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, 1, 1, 100);

    // enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

/* display function - GLUT display callback function */
void display(void)
{
    // clears the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // sets light colour and positions
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight2);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

    // set camera position
    gluLookAt(scaleFactor*terrain.getSize()/2, 25, -scaleFactor*terrain.getSize()/2,
              scaleFactor*terrain.getSize()/2, 0, scaleFactor*terrain.getSize()/2,
              0, 1, 0);

    // handle camera movement
    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glTranslatef(terrain.getSize()/2, 0, terrain.getSize()/2);
    glRotatef(xAxisRotation, 1, 0, 0);
    glRotatef(yAxisRotation, 0, 1, 0);
    glTranslatef(-terrain.getSize()/2, 0, -terrain.getSize()/2);
    terrain.drawTerrain(wireFrameMode == 2, quadStripMode);         // draw terrain
    glPopMatrix();

    glutSwapBuffers();  // used for double buffering
}

/* display function for second window (2D Representation) */
void display2(void)
{
    // clears screen
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    terrain.draw2DTerrain();    // draws 2D representation of terrain

    glutSwapBuffers();  // used for double buffering
}

/* prints control instructions to console */
void printInstructions()
{
    cout << "TERRAIN GENERATOR" << endl;
    cout << "~ Created by Susan Yuen ~" << endl << endl;

    cout << "* q or ESC = quit the program" << endl;
    cout << "* LEFT/RIGHT ARROW = rotates scene about the y axis" << endl;
    cout << "* UP/DOWN ARROW = rotates scene about the x axis" << endl;
    cout << "* w = toggles between solid view, wireframe, or both" << endl;
    cout << "* l = enables/disables lighting" << endl;
    cout << "* f = enables/disables flat/gouraud shading" << endl;
    cout << "* t = enables triangle mesh" << endl;
    cout << "* y = enables quad mesh" << endl;
    cout << "* o = decreases terrain size by 50 tiles (minimum 50x50)" << endl;
    cout << "* p = increases terrain size by 50 tiles (max 300x300)" << endl;
    cout << "* u, j, h, k = moves light source 1 (blue light) along the x-z plane" << endl;
    cout << "* g, b, v, n = moves light source 2 (amber light) along the x-z plane" << endl;
    cout << "* x = toggle height algorithm (circle algorithm, fault algorithm, particle deposition)" << endl;
    cout << "* r = randomizes the terrain" << endl << endl;

    cout << "* BONUS FEATURE: 2D Representation of Array (Displayed in second window)" << endl;
    cout << "* BONUS FEATURE: Fault Algorithm (toggle to it using x)" << endl;
    cout << "* BONUS FEATURE: Particle Deposition (toggle to it using x)" << endl << endl;
}

/* main function - program entry point */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);		//starts GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // set display mode

    printInstructions();

    // set up second window (2D representation of terrain)
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(1000, 50);
    window2 = glutCreateWindow("2D Representation of Terrain");      // second window
    glutDisplayFunc(display2);

    // set up main window
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    window1 = glutCreateWindow("yuens2 | 001416198 | Assignment 2");	//creates the window

    // register callback functions
    glutDisplayFunc(display);	// registers "display" as the display callback function
    glutKeyboardFunc(keyboard); // registers "keyboard" as the keyboard callback function
    glutSpecialFunc(special);   // registers "special" as the keyboard callback function

    glEnable(GL_DEPTH_TEST);    // enable depth test
    init();                     // calls init to set up OpenGL
    glutMainLoop();				// starts the event loop

    return(0);					// return may not be necessary on all compilers
}
