
// This is demo program for creating a car using OpenGL.
// The car can be run using control keys.
// Original contribution: Sai Kumar Minajagi
// Modified for the purpose of cpsc3710

#include <GL/glut.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define ESCAPE 27

typedef enum {
  BEHIND,
  RIGHT,
  LEFT,
  RESET,
  BR,
  FR,
  FL,
  BL,
  DBR,
  DFR,
  DFL,
  DBL
} CameraAngle;

GLint window;
GLint window2;
GLint Xsize = 1000;
GLint Ysize = 800;
GLint nml = 0;

bool turning = false;

int times = 0; // integer flag to fix rotation

bool forwardDown = false;
bool backDown = false;
bool leftDown = false;
bool rightDown = false;

bool r1[200];
bool r2[200];
bool r3[200];

int facing = 2;
int tfacing = 0;
int turnDir = 0;

float turnRot = 0;
float cS = 0.0325;

float eyex = 3.0f;
float eyey = 1.0f;
float eyez = 0.0f;

float eyeTx = -10.0f;
float eyeTy = 3.0f;
float eyeTz = 0.0f;

float carx = 0.2f;
float cary = 0.0f;
float carz = 0.5f;
float carOy = 0.5f;

float interSize = 1;
float roadSize = 10;

CameraAngle cam = RESET;

GLfloat xt = 0.0, yt = 0.0, zt = 0.0, xw = 0.0;
GLfloat xs = 1.0, ys = 1.0, zs = 1.0;
GLfloat xangle = 5.0, yangle = 0.0, zangle = 0.0, angle = 0.0;
float cRot = 0.0f, theta;

GLfloat r = 0, g = 0, b = 1;
GLUquadricObj *t;

static void SpecialKeyFunc (int Key, int x, int y);

GLvoid Transform (GLfloat Width, GLfloat Height) {
  glViewport (0, 0, Width, Height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (45.0, Width / Height, 0.1, 100.0);
  glMatrixMode (GL_MODELVIEW);
}

GLvoid InitGL (GLfloat Width, GLfloat Height) {

  glClearColor (1.0, 1.0, 1.0, 1.0);
  glLineWidth (2.0);             /* Add line width,   ditto */
  Transform (Width, Height);  /* Perform the transformation */
  t = gluNewQuadric();
  gluQuadricDrawStyle (t, GLU_FILL);

  glEnable (GL_LIGHTING);

  glEnable (GL_LIGHT0);

  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };

  glLightfv (GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv (GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv (GL_LIGHT0, GL_POSITION, position);

}

void init() {
  glClearColor (0, 0, 0, 0);
  glPointSize (5.0);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (0.0, 900.0, 0.0, 600.0, 50.0, -50.0);
  glutPostRedisplay();
}



GLvoid DrawGLScene() {
  float i;
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  InitGL (Xsize, Ysize);

  glClearColor (1, 1, 1, 1);

  glLoadIdentity();

  // camera stuff

  //*************************************************************************
  // Camera positions for all angles
  //*************************************************************************
  switch (cam) {
  case BEHIND:
    switch (tfacing) {
    case 0:
      eyeTx = 3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;

    case 1:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = -3.0f;
      break;

    case 2:
      eyeTx = -3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;

    case 3:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = 3.0f;
      break;
    }

    break;

  case RIGHT:
    switch (tfacing) {
    case 0:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = 3.0f;
      break;

    case 1:
      eyeTx = 3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;

    case 2:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = -3.0f;
      break;

    case 3:
      eyeTx = -3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;
    }

    break;

  case LEFT:
    switch (tfacing) {
    case 0:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = -3.0f;
      break;

    case 1:
      eyeTx = -3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;

    case 2:
      eyeTx = 0.0f;
      eyeTy = 1.0f;
      eyeTz = 3.0f;
      break;

    case 3:
      eyeTx = 3.0f;
      eyeTy = 1.0f;
      eyeTz = 0.0f;
      break;
    }

    break;
  }


  //*************************************************************************
  // Smooth camera handling, attempt to get eyex/eyey/eyez to target positions
  //*************************************************************************
  bool bx1 = false;
  bool bx2 = false;
  bool by1 = false;
  bool by2 = false;
  bool bz1 = false;
  bool bz2 = false;

  if (eyex < eyeTx - 0.1) {
    eyex += cS;
  } else {
    bx1 = true;
  }

  if (eyey < eyeTy - 0.1) {
    eyey += cS;
  } else {
    by1 = true;
  }

  if (eyez < eyeTz - 0.1) {
    eyez += cS;
  } else {
    bz1 = true;
  }

  if (eyex > eyeTx + 0.1) {
    eyex -= cS;
  } else {
    bx2 = true;
  }

  if (eyey > eyeTy + 0.1) {
    eyey -= cS;
  } else {
    by2 = true;
  }

  if (eyez > eyeTz + 0.1) {
    eyez -= cS;
  } else {
    bz2 = true;
  }

  if (bx1 && bx2)
    eyex = eyeTx;

  if (by1 && by2)
    eyey = eyeTy;

  if (bz1 && bz2)
    eyez = eyeTz;

  //*************************************************************************

  gluLookAt (eyex + carx, eyey + cary, eyez + carz, carx, cary + carOy, carz, 0, 1, 0);

  glEnable (GL_COLOR_MATERIAL);

  // For fog effect
  /*GLfloat fogcolour[4]={1.0,1.0,1.0,1.0};
  glFogfv(GL_FOG_COLOR,fogcolour);
  glFogf(GL_FOG_DENSITY,0.1);
  glFogi(GL_FOG_MODE,GL_EXP);
  glFogf(GL_FOG_START,3.0);
  glFogf(GL_FOG_END,100.0);
  glHint(GL_FOG_HINT, GL_FASTEST);
  glEnable(GL_FOG);*/

  glBegin (GL_QUADS);
  //*****************************road and surrounding development***********************************
  genRoads();
  glEnd();

  glBegin (GL_QUADS);
  genBuildings();
  glEnd();


  glPushMatrix();
  //glTranslatef(xw,0,0);
  //glColor3f(0.5,.2,0.3);
  glPopMatrix();

  //*************************************************************

  glPushMatrix();
  glTranslatef (carx, cary, carz);

  if (turning) {
    glRotatef (turnRot, 0, 1, 0);
    turnRot += turnDir;
    tfacing = (facing + turnDir) % 4;

    if (tfacing < 0)
      tfacing = 3;

    // detect if finished rotation
    float min = (90 * (facing + turnDir) ) - 1;
    float max = (90 * (facing + turnDir) ) + 1;

    printf("%f, %f, %f\n", min, max, turnRot);
    times += 1;
    if (times > 120)
      turnRot = 90 * (facing + turnDir);

    if (turnRot > min && turnRot < max) {
      times = 0;
      turning = false;
      facing = (facing + turnDir) % 4;

      if (facing < 0)
        facing = 3;

      printf ("Done turn, new facing: %d\n", facing);
    }

    // snap to road
    if (facing == 1 || facing == 3) {
      // snap to nearest on z axis
    } else if (facing == 0 || facing == 2) {
      // snap to nearest on x axis

    }
  } else {
    tfacing = facing;
    turnRot = 0 + (90 * facing);
    glRotatef (turnRot, 0, 1, 0);

    float fS = 0.07;
    float bS = 0.015;

    if (forwardDown) {
      switch (facing) {
      case 0:
        carx -= fS;
        break;

      case 1:
        carz += fS;
        break;

      case 2:
        carx += fS;
        break;

      case 3:
        carz -= fS;
        break;

      default:
        printf ("Error attempting to move: Facing is %d", facing);
      }
    } else if (backDown) {
      switch (facing) {
      case 0:
        carx += bS;
        break;

      case 1:
        carz -= bS;
        break;

      case 2:
        carx -= bS;
        break;

      case 3:
        carz += bS;
        break;

      default:
        printf ("Error attempting to move: Facing is %d", facing);
      }
    }
  }

  glTranslatef (-1.15, 0, -0.4);
  drawCar();
  glPopMatrix();

  glEnable (GL_DEPTH_TEST);
  glutPostRedisplay();
  glutSwapBuffers();

  cRot++;
}

void genBuildings() {
  int i, j;
  float hroad = roadSize / 2;

  // create grass plots and gen buildings inside them
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 20; j++) {
      float vert1x = hroad + (j * (roadSize + interSize) );
      float vert2x = -hroad + (j * (roadSize + interSize) );
      float vert1z = i * (roadSize + interSize) + interSize;
      float vert2z = vert1z + roadSize;

      glBegin (GL_QUADS);

      glColor3f (0, 1, 0);
      glVertex3f (vert1x, 0, vert1z);
      glVertex3f (vert2x, 0, vert1z);
      glVertex3f (vert2x, 0, vert2z);
      glVertex3f (vert1x, 0, vert2z);

      glEnd();

      // vert1x/vert2x/vert1z/vert2z are the corner vertices of plot.  Generate structures within.

      if (r1[20 * i + j]) {
        // gen cylinder
        glPushMatrix();

        glTranslatef (vert1x + 5, 3, vert1z + 5);
        cylinderBuilding();

        glPopMatrix();
      }

      if (r2[20 * i + j]) {
        // gen sphere
        glPushMatrix();

        glTranslatef (vert1x + 2, 1, vert1z + 7);
        sphereBuilding();

        glPopMatrix();
      }

      if (r3[20 * i + j]) {
        // gen cube
        glPushMatrix();

        glTranslatef (vert1x + 7, 1, vert1z + 2);
        squareBuilding();

        glPopMatrix();
      }
    }
  }
}

void genRoads() {
  int i, j, k;

  float hroad = roadSize / 2;

  // generate roads
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 20; j++) {
      for (k = 0; k < 20; k++) {
        float vert1x = hroad  + (k * (roadSize + interSize) );
        float vert2x = -hroad + (k * (roadSize + interSize) );
        float vertz  = j * (roadSize + interSize);
        glBegin (GL_QUADS);
        glColor3f (0.7, 0.7, 0.7);
        glVertex3f (vert2x, 0, 0    + vertz);
        glVertex3f (vert2x, 0, 0.45 + vertz);        //a long road
        glVertex3f (vert1x, 0, 0.45 + vertz);
        glVertex3f (vert1x, 0, 0    + vertz);

        glColor3f (1.0, 0.75, 0.0);
        glVertex3f (vert2x, 0, 0.45 + vertz);      //a median
        glVertex3f (vert2x, 0, 0.55 + vertz);
        glVertex3f (vert1x, 0, 0.55 + vertz);
        glVertex3f (vert1x, 0, 0.45 + vertz);

        glColor3f (0.7, 0.7, 0.7);
        glVertex3f (vert2x, 0, 0.55 + vertz);
        glVertex3f (vert2x, 0, 1    + vertz);           //a long road
        glVertex3f (vert1x, 0, 1    + vertz);
        glVertex3f (vert1x, 0, 0.55 + vertz);
        glEnd();
      }
    }

    if (i < 1) { // only run on the first iteration
      glPushMatrix();
      glRotatef (-90, 0, 1, 0);
      glRotatef (180, 1, 0, 0);
      glTranslatef (hroad + interSize, 0, hroad);
    }
  }

  // reverse translations
  glTranslatef (- (hroad + interSize), 0, -hroad);
  glRotatef (-180, 1, 0, 0);
  glRotatef (90, 0, 1, 0);

  glPopMatrix();

  // generate intersections
  for (j = 0; j < 20; j++) {
    for (k = 0; k < 20; k++) {
      float vertx = k * (roadSize + interSize);
      float vertz = j * (roadSize + interSize);
      glBegin (GL_QUADS);
      glColor3f (0.7, 0.7, 0.7);
      glVertex3f (hroad             + vertx, 0, 0         + vertz);
      glVertex3f (hroad             + vertx, 0, interSize + vertz);
      glVertex3f (hroad + interSize + vertx, 0, interSize + vertz);
      glVertex3f (hroad + interSize + vertx, 0, 0         + vertz);
      glEnd();
    }
  }

  glPopMatrix();
}

void sphereBuilding() {
  glPushMatrix();

  glBegin (GL_QUADS);
  glColor3f (0, 0, 0);
  glutSolidSphere (1, 20, 20);

  glRotatef (cRot, 0.0f, 1.0f, 0.0f);
  glTranslatef (0, 0.8, 0.5);

  //Antenna
  glRotatef (45, -1, 0, 0);
  glColor3f (1, 0, 0);
  gluCylinder (t, 0.2, 0.2, .2, 10, 10); //Main cylinder
  gluCylinder (t, 0.2, 0, 0, 10, 10); //Top
  glTranslatef (0, 0, 0.2);
  gluCylinder (t, 0, 0.2, 0, 10, 10); //Bottom
  glEnd();

  glPopMatrix();
}

void cylinderBuilding() {
  glPushMatrix();
  // cylinder
  glColor3f (1, 0, 0);
  glRotatef (90.0, 1.0, 0.0, 0.0); //x
  gluCylinder (t, 1, 1, 3, 64, 64);

  // cube at top
  glRotatef (cRot, 0, 0, 1); //y
  glutSolidCube (2);

  glEnd();
  //glPopMatrix();

  glPopMatrix();
}

void squareBuilding() {
  glPushMatrix();

  glBegin (GL_QUADS);
  glColor3f (0.2, 0.2, 0.2);
  glutSolidCube (2);

  glColor3f (0.1, 0.1, 1);
  glTranslatef (0.76, 0, 0);
  glutSolidCube (0.5);

  glTranslatef (-1.43, 0, 0);

  glRotatef (90, 1, 0, 0);
  glRotatef (45, 0, 0, 1);
  gluCylinder (t, 0.5, 0.5, 0.2, 4, 10);

  glTranslatef (1.43, -0.19, 0);
  gluCylinder (t, 0.2, 0.2, 0.5, 4, 10);

  glTranslatef (-1.3, -1, -0.5);
  gluCylinder (t, 0.4, 0.4, 0.8, 4, 10);

  glEnd();

  glPopMatrix();
}

void drawCar() {

  glColor3f (1.0, .75, 0.0);
  glPointSize (30.0);
  glBegin (GL_POINTS);
  glVertex3f (0.2, 0.3, 0.3);
  glVertex3f (0.2, 0.3, 0.5);
  glEnd();
  glPointSize (200.0);
  glBegin (GL_QUADS);
  /* top of cube*/
  //************************FRONT BODY****************************************
  glColor3f (r, g, b);
  glVertex3f (0.2, 0.4, 0.6);
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.6, 0.5, 0.2);
  glVertex3f (0.2, 0.4, 0.2);
  /* bottom of cube*/
  glVertex3f (0.2, 0.4, 0.6);
  glVertex3f (0.6, 0.2, 0.6);
  glVertex3f (0.6, 0.2, 0.2);
  glVertex3f (0.2, 0.2, 0.2);
  /* front of cube*/
  glVertex3f (0.2, 0.2, 0.6);
  glVertex3f (0.2, 0.4, 0.6);
  glVertex3f (0.2, 0.4, 0.2);
  glVertex3f (0.2, 0.2, 0.2);
  /* back of cube.*/
  glVertex3f (0.6, 0.2, 0.6);
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.6, 0.5, 0.2);
  glVertex3f (0.6, 0.2, 0.2);
  /* left of cube*/
  glVertex3f (0.2, 0.2, 0.6);
  glVertex3f (0.6, 0.2, 0.6);
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.2, 0.4, 0.6);
  /* Right of cube */
  glVertex3f (0.2, 0.2, 0.2);
  glVertex3f (0.6, 0.2, 0.2);
  glVertex3f (0.6, 0.5, 0.2);
  glVertex3f (0.2, 0.4, 0.2);

  //****************************************************************************
  glVertex3f (0.7, 0.65, 0.6);
  glVertex3f (0.7, 0.65, 0.2);
  glVertex3f (1.7, 0.65, 0.2);
  glVertex3f (1.7, 0.65, 0.6);

  //***************************back guard******************************
  // You can change r, g, b to different colors.
  glColor3f (r, g, b);
  glVertex3f (1.8, 0.5, 0.6);
  glVertex3f (1.8, 0.5, 0.2);
  glVertex3f (2.1, 0.4, 0.2);
  glVertex3f (2.1, 0.4, 0.6);
  /* bottom of cube*/
  glVertex3f (2.1, 0.2, 0.6);
  glVertex3f (2.1, 0.2, 0.2);
  glVertex3f (1.8, 0.2, 0.6);
  glVertex3f (1.8, 0.2, 0.6);
  /* back of cube.*/
  glVertex3f (2.1, 0.4, 0.6);
  glVertex3f (2.1, 0.4, 0.2);
  glVertex3f (2.1, 0.2, 0.2);
  glVertex3f (2.1, 0.2, 0.6);
  /* left of cube*/
  glVertex3f (1.8, 0.2, 0.2);
  glVertex3f (1.8, 0.5, 0.2);
  glVertex3f (2.1, 0.4, 0.2);
  glVertex3f (2.1, 0.2, 0.2);
  /* Right of cube */
  glVertex3f (1.8, 0.2, 0.6);
  glVertex3f (1.8, 0.5, 0.6);
  glVertex3f (2.1, 0.4, 0.6);
  glVertex3f (2.1, 0.2, 0.6);

  //******************MIDDLE BODY************************************
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.6, 0.2, 0.6);
  glVertex3f (1.8, 0.2, 0.6);
  glVertex3f (1.8, 0.5, 0.6);

  /* bottom of cube*/
  glVertex3f (0.6, 0.2, 0.6);
  glVertex3f (0.6, 0.2, 0.2);
  glVertex3f (1.8, 0.2, 0.2);
  glVertex3f (1.8, 0.2, 0.6);

  /* back of cube.*/
  glVertex3f (0.6, 0.5, 0.2);
  glVertex3f (0.6, 0.2, 0.2);
  glVertex3f (1.8, 0.2, 0.2);
  glVertex3f (1.8, 0.5, 0.2);

  //*********************ENTER WINDOW**********************************
  glColor3f (0.3, 0.3, 0.3);
  glVertex3f (0.77, 0.63, 0.2);
  glVertex3f (0.75, 0.5, 0.2);      //quad front window
  glVertex3f (1.2, 0.5, 0.2);
  glVertex3f (1.22, 0.63, 0.2);

  glVertex3f (1.27, 0.63, .2);
  glVertex3f (1.25, 0.5, 0.2);     //quad back window
  glVertex3f (1.65, 0.5, 0.2);
  glVertex3f (1.67, 0.63, 0.2);

  glColor3f (r, g, b);
  glVertex3f (0.7, 0.65, 0.2);
  glVertex3f (0.7, 0.5, .2);    //first separation
  glVertex3f (0.75, 0.5, 0.2);
  glVertex3f (0.77, 0.65, 0.2);

  glVertex3f (1.2, 0.65, 0.2);
  glVertex3f (1.2, 0.5, .2);    //second separation
  glVertex3f (1.25, 0.5, 0.2);
  glVertex3f (1.27, 0.65, 0.2);

  glVertex3f (1.65, 0.65, 0.2);
  glVertex3f (1.65, 0.5, .2);  //3d separation
  glVertex3f (1.7, 0.5, 0.2);
  glVertex3f (1.7, 0.65, 0.2);

  glVertex3f (0.75, 0.65, 0.2);
  glVertex3f (0.75, 0.63, 0.2);      //line strip
  glVertex3f (1.7, 0.63, 0.2);
  glVertex3f (1.7, 0.65, 0.2);

  glVertex3f (0.75, 0.65, 0.6);
  glVertex3f (0.75, 0.63, 0.6);      //line strip
  glVertex3f (1.7, 0.63, 0.6);
  glVertex3f (1.7, 0.65, 0.6);

  glColor3f (0.3, 0.3, 0.3);
  glVertex3f (0.77, 0.63, 0.6);
  glVertex3f (0.75, 0.5, 0.6);      //quad front window
  glVertex3f (1.2, 0.5, 0.6);
  glVertex3f (1.22, 0.63, 0.6);

  glVertex3f (1.27, 0.63, .6);
  glVertex3f (1.25, 0.5, 0.6);     //quad back window
  glVertex3f (1.65, 0.5, 0.6);
  glVertex3f (1.67, 0.63, 0.6);

  glColor3f (r, g, b);
  glVertex3f (0.7, 0.65, 0.6);
  glVertex3f (0.7, 0.5, .6);    //first separation
  glVertex3f (0.75, 0.5, 0.6);
  glVertex3f (0.77, 0.65, 0.6);

  glVertex3f (1.2, 0.65, 0.6);
  glVertex3f (1.2, 0.5, .6);    //second separation
  glVertex3f (1.25, 0.5, 0.6);
  glVertex3f (1.27, 0.65, 0.6);

  glVertex3f (1.65, 0.65, 0.6);
  glVertex3f (1.65, 0.5, .6);
  glVertex3f (1.7, 0.5, 0.6);
  glVertex3f (1.7, 0.65, 0.6);
  glEnd();


  //**************************************************************
  glBegin (GL_QUADS);

  /* top of cube*/
  glColor3f (0.3, 0.3, 0.3);
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.6, 0.5, 0.2);      //quad front window
  glVertex3f (0.7, 0.65, 0.2);
  glVertex3f (0.7, 0.65, 0.6);

  glVertex3f (1.7, 0.65, .6);
  glVertex3f (1.7, 0.65, 0.2);     //quad back window
  glVertex3f (1.8, 0.5, 0.2);
  glVertex3f (1.8, 0.5, 0.6);
  glEnd();
  glBegin (GL_TRIANGLES);               /* start drawing the cube.*/
  /* top of cube*/
  glColor3f (0.3, 0.3, 0.3);
  glVertex3f (0.6, 0.5, 0.6);
  glVertex3f (0.7, 0.65, 0.6);     //tri front window
  glVertex3f (0.7, 0.5, 0.6);

  glVertex3f (0.6, 0.5, 0.2);
  glVertex3f (0.7, 0.65, 0.2);     //tri front window
  glVertex3f (0.7, 0.5, 0.2);

  glVertex3f (1.7, 0.65, 0.2);
  glVertex3f (1.8, 0.5, 0.2);     //tri back window
  glVertex3f (1.7, 0.5, 0.2);

  glVertex3f (1.7, 0.65, 0.6);
  glVertex3f (1.8, 0.5, 0.6);     //tri back window
  glVertex3f (1.7, 0.5, 0.6);
  glEnd();

  //************IGNITION SYSTEM
  glPushMatrix();
  glColor3f (0.3, 0.3, 0.7);
  glTranslatef (1.65, 0.2, 0.3);
  glRotatef (90.0, 0, 1, 0);
  gluCylinder (t, 0.02, 0.03, .5, 10, 10);
  glPopMatrix();

  //********************WHEEL
  glColor3f (0.7, 0.7, 0.7);
  glPushMatrix();
  glBegin (GL_LINE_STRIP);

  for (theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f (0.6, 0.2, 0.62);
    glVertex3f (0.6 + (0.08 * (cos ( ( (theta + angle) * 3.14) / 180) ) ), 0.2 + (0.08 * (sin ( ( (theta + angle) * 3.14) / 180) ) ), 0.62);
  }

  glEnd();

  glBegin (GL_LINE_STRIP);

  for (theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f (0.6, 0.2, 0.18);
    glVertex3f (0.6 + (0.08 * (cos ( ( (theta + angle) * 3.14) / 180) ) ), 0.2 + (0.08 * (sin ( ( (theta + angle) * 3.14) / 180) ) ), 0.18);
  }

  glEnd();

  glBegin (GL_LINE_STRIP);

  for (theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f (1.7, 0.2, 0.18);
    glVertex3f (1.7 + (0.08 * (cos ( ( (theta + angle) * 3.14) / 180) ) ), 0.2 + (0.08 * (sin ( ( (theta + angle) * 3.14) / 180) ) ), 0.18);
  }

  glEnd();

  glBegin (GL_LINE_STRIP);

  for (theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f (1.7, 0.2, 0.62);
    glVertex3f (1.7 + (0.08 * (cos ( ( (theta + angle) * 3.14) / 180) ) ), 0.2 + (0.08 * (sin ( ( (theta + angle) * 3.14) / 180) ) ), 0.62);
  }

  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslatef (0.6, 0.2, 0.6);
  glColor3f (0, 0, 0);
  glutSolidTorus (0.025, 0.07, 10, 25);

  glTranslatef (0, 0, -0.4);
  glutSolidTorus (0.025, 0.07, 10, 25);

  glTranslatef (1.1, 0, 0);
  glutSolidTorus (0.025, 0.07, 10, 25);

  glTranslatef (0, 0, 0.4);
  glutSolidTorus (0.025, 0.07, 10, 25);
  glPopMatrix();

  /******************AANTENA**********************************/
  glPushMatrix();
  glColor3f (0.7, 0, 0.7);
  glTranslatef (2.0, 0.6, 0.4);
  glRotatef (cRot, 0.0f, 1.0f, 0.0f);
  glRotatef (90.0, 1, 0, 0);
  gluCylinder (t, 0.02, 0.03, .2, 10, 10);
  glTranslatef (0, 0, 0.15);
  glutSolidSphere (0.04, 10, 10);
  glTranslatef (0, -0.025, -0.15);
  glutSolidSphere (0.04, 10, 10);
  glTranslatef (0, 0.05, 0);
  glutSolidSphere (0.04, 10, 10);
  glPopMatrix();


  /*********************ROCKETS*****************************/
  glPushMatrix();
  glColor3f (0.3, 0.3, 0.7);
  glTranslatef (0.65, 0.4, 0.67);
  glRotatef (90.0, 0, 1, 0);
  gluCylinder (t, 0.06, 0.06, .2, 10, 10);
  gluCylinder (t, 0.06, 0, .2, 10, 10);
  glTranslated (0, 0, 0.19);
  gluCylinder (t, 0.06, 0, .2, 10, 10);

  glTranslatef (0.54, 0, -0.19);
  gluCylinder (t, 0.06, 0.06, .2, 10, 10);
  gluCylinder (t, 0.06, 0, .2, 10, 10);
  glTranslated (0, 0, 0.19);
  gluCylinder (t, 0.06, 0, .2, 10, 10);
  glPopMatrix();

  /********************REARLIGHTS***************************/
  glPushMatrix();
  glColor3f (1.0, 0.0, 0.0);
  glTranslatef (1.9, 0, 0);
  glPointSize (30.0);
  glBegin (GL_POINTS);
  glVertex3f (0.2, 0.3, 0.3);
  glVertex3f (0.2, 0.3, 0.5);
  glEnd();
  glPopMatrix();

  /*******************LICENSE*PLATE************************/
  char text[] = "CPSC 3710";
  glBegin (GL_QUADS);
  glNormal3f (0.0f, 0.0f, -1.0f);
  glColor3f (0.2, 0.2, 0.2);
  glTexCoord2f (0.995f, 0.005f);
  glVertex3f (0.2f, 0.25f, 0.35f);
  glTexCoord2f (2.995f, 2.995f);
  glVertex3f (0.2f,  0.35f, 0.35f);
  glTexCoord2f (0.005f, 0.995f);
  glVertex3f (0.2f,  0.35f, 0.45f);
  glTexCoord2f (0.005f, 0.005f);
  glVertex3f (0.2f, 0.25f, 0.45f);
  glEnd();
}

void NormalKeyUp (GLubyte key, GLint x, GLint y) {
  switch (key) {
  case 'i': // forward
    forwardDown = false;
    break;

  case 'k': // back
    backDown = false;
  }
}

bool atIntersection() {
  printf ("X: %f\n", fmod (carx, (roadSize + 1) ) );
  printf ("Z: %f\n", fmod (carz, (roadSize + 1) ) );

  switch (facing) {
  case 0:
  case 2:
    if (fmod (carx, (roadSize + interSize) ) >= 5.4 && fmod (carx, (roadSize + interSize) ) <= 5.6) {
      return true;
    }

    break;

  case 1:
  case 3:
    if (fmod (carz, (roadSize + interSize) ) >= 0.4 && fmod (carz, (roadSize + interSize) ) <= 0.6) {
      return true;
    }

  default:
    return false;
  }


  return false;
}

void NormalKey (GLubyte key, GLint x, GLint y) {
  switch (key)    {
  case ESCAPE :
    printf ("escape pressed. exit.\n");
    glutDestroyWindow (window);
    exit (0);
    break;

  case 'i': // forward
    forwardDown = true;
    break;

  case 'k': // backward
    backDown = true;
    break;

  case 'j': // left
    if (!turning && atIntersection() ) {
      turning = true;
      turnDir = 1;
    }

    break;

  case 'l': // right
    if (!turning && atIntersection() ) {
      turning = true;
      turnDir = -1;
    }

    break;

  default:
    break;
  }
}

// GLUT_KEY_F[1..12]
static void SpecialKeyFunc (int Key, int x, int y) {
  switch (Key) {
  case GLUT_KEY_F1:
    cam = BEHIND;
    break;

  case GLUT_KEY_F2:
    cam = RIGHT;
    break;

  case GLUT_KEY_F3:
    cam = LEFT;
    break;

  case GLUT_KEY_F4:
    cam = RESET;
    break;

  case GLUT_KEY_F5:
    cam = BR;
    break;

  case GLUT_KEY_F6:
    cam = FR;
    break;

  case GLUT_KEY_F7:
    cam = FL;
    break;

  case GLUT_KEY_F8:
    cam = BL;
    break;

  case GLUT_KEY_F9:
    cam = DBR;
    break;

  case GLUT_KEY_F10:
    cam = DFR;
    break;

  case GLUT_KEY_F11:
    cam = DFL;
    break;

  case GLUT_KEY_F12:
    cam = DBL;
    break;
  }
}

void colorMenu (int id) {
  switch (id) {
  case 6:
    r = g = 0;
    b = 1;
    glutPostRedisplay();
    break;

  case 7:
    r = 0.8;
    b = g = 0;
    glutPostRedisplay();
    break;

  case 8:
    g = 1;
    r = b = 0;
    glutPostRedisplay();
    break;

  case 9:
    r = b = g = 0;
    glutPostRedisplay();
    break;

  case 10:
    b = 0;
    r = g = 1;
    glutPostRedisplay();
    break;

  case 11:
    b = r = g = .7;
    glutPostRedisplay();
    break;

  default:
    break;
  }
}

int main (int argc, char **argv) {
  printf ("Car Animation Demo for CPSC3710\n");
  printf ("MOUSE\n");
  printf ("\tPRESS RIGHT BUTTON FOR MENU\n");
  printf ("KEYBOARD\n");
  printf ("\tX-Y-Z KEYS FOR CORRESPONDING ROTATION\n");
  printf ("\tU-F FOR CAMERA VIEW SETTINGS\n");
  printf ("\tUSE LEFT ARROW(<-) AND RIGHT ARROW(->) TO MOVE CAR\n");
  printf ("\tESCAPE TO EXIT\n");

  srand (time (0) );

  int i;

  for (int i = 0; i < 200; i++) {
    r1[i] = rand() % 5 == 0 ? true : false;
    r2[i] = rand() % 5 == 0 ? true : false;
    r3[i] = rand() % 5 == 0 ? true : false;
  }

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize (Xsize, Ysize);
  glutInitWindowPosition (50, 50);
  glutCreateWindow ("3D CAR ANIMATION");
  init();
  glutDisplayFunc (DrawGLScene);
  glutKeyboardFunc (NormalKey);
  glutKeyboardUpFunc (NormalKeyUp);
  glutSpecialFunc (SpecialKeyFunc);
  InitGL (Xsize, Ysize);

  glutCreateMenu (colorMenu);
  // You can change the color of the car
  glutAddMenuEntry ("blue", 6);
  glutAddMenuEntry ("red", 7);
  glutAddMenuEntry ("green", 8);
  glutAddMenuEntry ("black", 9);
  glutAddMenuEntry ("yellow", 10);
  glutAddMenuEntry ("grey", 11);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 1;
}
