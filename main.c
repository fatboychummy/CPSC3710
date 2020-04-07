
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

#define ESCAPE 27

GLint window;
GLint window2;
GLint Xsize = 1000;
GLint Ysize = 800;
GLint nml = 0;

bool turning = false;
bool atIntersection = false;

bool forwardDown = false;
bool backDown = false;
bool leftDown = false;
bool rightDown = false;

int facing = 0;
int turnDir = 0;

float turnRot = 0;

float eyex = -10.0f;
float eyey = 3.0f;
float eyez = 0.0f;

float carx = 0.0f;
float cary = 0.0f;
float carz = 0.0f;

GLfloat xt = 0.0, yt = 0.0, zt = 0.0, xw = 0.0;
GLfloat xs = 1.0, ys = 1.0, zs = 1.0;
GLfloat xangle = 5.0, yangle = 0.0, zangle = 0.0, angle = 0.0;
float cRot = 0.0f, theta;

GLfloat r = 0, g = 0, b = 1;
GLUquadricObj *t;

static void SpecialKeyFunc(int Key, int x, int y);

GLvoid Transform(GLfloat Width, GLfloat Height) {
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, Width / Height, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

GLvoid InitGL(GLfloat Width, GLfloat Height) {

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);              /* Add line width,   ditto */
  Transform(Width, Height);   /* Perform the transformation */
  t = gluNewQuadric();
  gluQuadricDrawStyle(t, GLU_FILL);

  glEnable(GL_LIGHTING);

  glEnable(GL_LIGHT0);

  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

}

void init() {
  glClearColor(0, 0, 0, 0);
  glPointSize(5.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 900.0, 0.0, 600.0, 50.0, -50.0);
  glutPostRedisplay();
}



GLvoid DrawGLScene() {
  float i;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  InitGL(Xsize, Ysize);

  glClearColor(1, 1, 1, 1);

  glLoadIdentity();

  gluLookAt(eyex + carx, eyey + cary, eyez + carz, carx, cary, carz, 0, 1, 0);
  /*
  glTranslatef(-1.0,0.0,-3.5);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glTranslatef(xt,yt,zt);
  glScalef(xs,ys,zs);
  */
  glEnable(GL_COLOR_MATERIAL);

  // For fog effect
  /*GLfloat fogcolour[4]={1.0,1.0,1.0,1.0};
  glFogfv(GL_FOG_COLOR,fogcolour);
  glFogf(GL_FOG_DENSITY,0.1);
  glFogi(GL_FOG_MODE,GL_EXP);
  glFogf(GL_FOG_START,3.0);
  glFogf(GL_FOG_END,100.0);
  glHint(GL_FOG_HINT, GL_FASTEST);
  glEnable(GL_FOG);*/

  glBegin(GL_QUADS);
  //*****************************road and surrounding development***********************************
  glPushMatrix();
  glTranslatef(xw, 0, 0);
  genRoads();
  glPopMatrix();
  glEnd();

  glPushMatrix();
  //glTranslatef(xw,0,0);
  //glColor3f(0.5,.2,0.3);
  glPopMatrix();

  //*************************************************************

  glPushMatrix();
  glTranslatef(carx, cary, carz);
  if(turning) {
    glRotatef(turnRot, 0, 1, 0);
    turnRot += turnDir;
    float min = (90 * (facing + turnDir)) - 1;
    float max = (90 * (facing + turnDir)) + 1;
    if(turnRot > min && turnRot < max) {
      turning = false;
      facing = (facing + turnDir) % 4;
      if(facing < 0)
        facing = 3;
    }
  } else {
    turnRot = 0 + (90 * facing);
    glRotatef(turnRot, 0, 1, 0);

    float fS = 0.07;
    float bS = 0.015;
    if(forwardDown) {
      switch(facing) {
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
        printf("Error attempting to move: Facing is %d", facing);
      }
    } else if(backDown) {
      switch(facing) {
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
        printf("Error attempting to move: Facing is %d", facing);
      }
    }
  }
  glTranslatef(-1.15, 0, -0.4);
  drawCar();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 3, -5);
  cylinderBuilding();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(5, 1, 3);
  squareBuilding();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-15, 1, -10);
  sphereBuilding();
  glTranslatef(15, -1, -0);
  glPopMatrix();

  glEnable(GL_DEPTH_TEST);
  glutPostRedisplay();
  glutSwapBuffers();

  cRot++;
}

void genRoads() {
  int i, j, k;
  float interSize = 1;

  // generate roads
  for(i = 0; i < 2; i++) {
    for(j = 0; j < 20; j++) {
      for(k = 0; k < 20; k++) {
        glBegin(GL_QUADS);
        glColor3f(0.7, 0.7, 0.7);
        glVertex3f(-10 + (k * (20 + interSize)), 0, 0 + (j * (20 + interSize)));
        glVertex3f(-10 + (k * (20 + interSize)), 0, 0.45 + (j * (20 + interSize)));         //a long road
        glVertex3f(10 + (k * (20 + interSize)), 0, 0.45 + (j * (20 + interSize)));
        glVertex3f(10 + (k * (20 + interSize)), 0, 0 + (j * (20 + interSize)));

        glColor3f(1.0, 0.75, 0.0);
        glVertex3f(-10 + (k * (20 + interSize)), 0, 0.45 + (j * (20 + interSize)));       //a median
        glVertex3f(-10 + (k * (20 + interSize)), 0, 0.55 + (j * (20 + interSize)));
        glVertex3f(10 + (k * (20 + interSize)), 0, 0.55 + (j * (20 + interSize)));
        glVertex3f(10 + (k * (20 + interSize)), 0, 0.45 + (j * (20 + interSize)));

        glColor3f(0.7, 0.7, 0.7);
        glVertex3f(-10 + (k * (20 + interSize)), 0, 0.55 + (j * (20 + interSize)));
        glVertex3f(-10 + (k * (20 + interSize)), 0, 1 + (j * (20 + interSize)));            //a long road
        glVertex3f(10 + (k * (20 + interSize)), 0, 1 + (j * (20 + interSize)));
        glVertex3f(10 + (k * (20 + interSize)), 0, 0.55 + (j * (20 + interSize)));
        glEnd();
      }
    }
    if(i < 1) {  // only run on the first iteration
      glPushMatrix();
      glRotatef(-90, 0, 1, 0);
      glRotatef(180, 1, 0, 0);
      glTranslatef(10 + interSize, 0, 10);
    }
  }

  // reverse translations
  glTranslatef(-(10 + interSize), 0, -10);
  glRotatef(-180, 1, 0, 0);
  glRotatef(90, 0, 1, 0);

  glPopMatrix();

  // generate intersections
  for(i = 0; i < 2; i++) {
    for(j = 0; j < 20; j++) {
      for(k = 0; k < 20; k++) {
        glBegin(GL_QUADS);
        glColor3f(0.7, 0.7, 0.7);
        glVertex3f(10 + (k * (20 + interSize)), 0, 0 + (j * (20 + interSize)));
        glVertex3f(10 + (k * (20 + interSize)), 0, 1 + (j * (20 + interSize)));
        glVertex3f(11 + (k * (20 + interSize)), 0, 1 + (j * (20 + interSize)));
        glVertex3f(11 + (k * (20 + interSize)), 0, 0 + (j * (20 + interSize)));
        glEnd();
      }
    }
    if(i < 1) {  // only run on the first iteration
      glPushMatrix();
      glRotatef(-90, 0, 1, 0);
      glRotatef(180, 1, 0, 0);
      glTranslatef(10 + interSize, 0, 10);
    }
  }
  glTranslatef(-(10 + interSize), 0, -10);
  glRotatef(-180, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  glPopMatrix();
}

void sphereBuilding() {
  glPushMatrix();

  glBegin(GL_QUADS);
  glColor3f(0, 0, 0);
  glutSolidSphere(1, 20, 20);

  glRotatef(cRot, 0.0f, 1.0f, 0.0f);
  glTranslatef(0, 0.8, 0.5);

  //Antenna
  glRotatef(45, -1, 0, 0);
  glColor3f(1, 0, 0);
  gluCylinder(t, 0.2, 0.2, .2, 10, 10); //Main cylinder
  gluCylinder(t, 0.2, 0, 0, 10, 10); //Top
  glTranslatef(0, 0, 0.2);
  gluCylinder(t, 0, 0.2, 0, 10, 10); //Bottom
  glEnd();

  glPopMatrix();
}

void cylinderBuilding() {
  glPushMatrix();
  // cylinder
  glColor3f(1, 0, 0);
  glRotatef(90.0, 1.0, 0.0, 0.0); //x
  gluCylinder(t, 1, 1, 3, 64, 64);

  // cube at top
  glRotatef(cRot, 0, 0, 1); //y
  glutSolidCube(2);

  glEnd();
  //glPopMatrix();

  glPopMatrix();
}

void squareBuilding() {
  glPushMatrix();

  glBegin(GL_QUADS);
  glColor3f(0.2, 0.2, 0.2);
  glutSolidCube(2);

  glColor3f(0.1, 0.1, 1);
  glTranslatef(0.76, 0, 0);
  glutSolidCube(0.5);

  glTranslatef(-1.43, 0, 0);

  glRotatef(90, 1, 0, 0);
  glRotatef(45, 0, 0, 1);
  gluCylinder(t, 0.5, 0.5, 0.2, 4, 10);

  glTranslatef(1.43, -0.19, 0);
  gluCylinder(t, 0.2, 0.2, 0.5, 4, 10);

  glTranslatef(-1.3, -1, -0.5);
  gluCylinder(t, 0.4, 0.4, 0.8, 4, 10);

  glEnd();

  glPopMatrix();
}

void drawCar() {

  glColor3f(1.0, .75, 0.0);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.2, 0.3, 0.3);
  glVertex3f(0.2, 0.3, 0.5);
  glEnd();
  glPointSize(200.0);
  glBegin(GL_QUADS);
  /* top of cube*/
  //************************FRONT BODY****************************************
  glColor3f(r, g, b);
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.2, 0.4, 0.2);
  /* bottom of cube*/
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(0.2, 0.2, 0.2);
  /* front of cube*/
  glVertex3f(0.2, 0.2, 0.6);
  glVertex3f(0.2, 0.4, 0.6);
  glVertex3f(0.2, 0.4, 0.2);
  glVertex3f(0.2, 0.2, 0.2);
  /* back of cube.*/
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.6, 0.2, 0.2);
  /* left of cube*/
  glVertex3f(0.2, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.2, 0.4, 0.6);
  /* Right of cube */
  glVertex3f(0.2, 0.2, 0.2);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.2, 0.4, 0.2);

  //****************************************************************************
  glVertex3f(0.7, 0.65, 0.6);
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(1.7, 0.65, 0.2);
  glVertex3f(1.7, 0.65, 0.6);

  //***************************back guard******************************
  // You can change r, g, b to different colors.
  glColor3f(r, g, b);
  glVertex3f(1.8, 0.5, 0.6);
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.4, 0.6);
  /* bottom of cube*/
  glVertex3f(2.1, 0.2, 0.6);
  glVertex3f(2.1, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.2, 0.6);
  /* back of cube.*/
  glVertex3f(2.1, 0.4, 0.6);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.2, 0.2);
  glVertex3f(2.1, 0.2, 0.6);
  /* left of cube*/
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(2.1, 0.4, 0.2);
  glVertex3f(2.1, 0.2, 0.2);
  /* Right of cube */
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.5, 0.6);
  glVertex3f(2.1, 0.4, 0.6);
  glVertex3f(2.1, 0.2, 0.6);

  //******************MIDDLE BODY************************************
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8, 0.5, 0.6);

  /* bottom of cube*/
  glVertex3f(0.6, 0.2, 0.6);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.6);

  /* back of cube.*/
  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.6, 0.2, 0.2);
  glVertex3f(1.8, 0.2, 0.2);
  glVertex3f(1.8, 0.5, 0.2);

  //*********************ENTER WINDOW**********************************
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.77, 0.63, 0.2);
  glVertex3f(0.75, 0.5, 0.2);       //quad front window
  glVertex3f(1.2, 0.5, 0.2);
  glVertex3f(1.22, 0.63, 0.2);

  glVertex3f(1.27, 0.63, .2);
  glVertex3f(1.25, 0.5, 0.2);      //quad back window
  glVertex3f(1.65, 0.5, 0.2);
  glVertex3f(1.67, 0.63, 0.2);

  glColor3f(r, g, b);
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(0.7, 0.5, .2);     //first separation
  glVertex3f(0.75, 0.5, 0.2);
  glVertex3f(0.77, 0.65, 0.2);

  glVertex3f(1.2, 0.65, 0.2);
  glVertex3f(1.2, 0.5, .2);     //second separation
  glVertex3f(1.25, 0.5, 0.2);
  glVertex3f(1.27, 0.65, 0.2);

  glVertex3f(1.65, 0.65, 0.2);
  glVertex3f(1.65, 0.5, .2);   //3d separation
  glVertex3f(1.7, 0.5, 0.2);
  glVertex3f(1.7, 0.65, 0.2);

  glVertex3f(0.75, 0.65, 0.2);
  glVertex3f(0.75, 0.63, 0.2);       //line strip
  glVertex3f(1.7, 0.63, 0.2);
  glVertex3f(1.7, 0.65, 0.2);

  glVertex3f(0.75, 0.65, 0.6);
  glVertex3f(0.75, 0.63, 0.6);       //line strip
  glVertex3f(1.7, 0.63, 0.6);
  glVertex3f(1.7, 0.65, 0.6);

  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.77, 0.63, 0.6);
  glVertex3f(0.75, 0.5, 0.6);       //quad front window
  glVertex3f(1.2, 0.5, 0.6);
  glVertex3f(1.22, 0.63, 0.6);

  glVertex3f(1.27, 0.63, .6);
  glVertex3f(1.25, 0.5, 0.6);      //quad back window
  glVertex3f(1.65, 0.5, 0.6);
  glVertex3f(1.67, 0.63, 0.6);

  glColor3f(r, g, b);
  glVertex3f(0.7, 0.65, 0.6);
  glVertex3f(0.7, 0.5, .6);     //first separation
  glVertex3f(0.75, 0.5, 0.6);
  glVertex3f(0.77, 0.65, 0.6);

  glVertex3f(1.2, 0.65, 0.6);
  glVertex3f(1.2, 0.5, .6);     //second separation
  glVertex3f(1.25, 0.5, 0.6);
  glVertex3f(1.27, 0.65, 0.6);

  glVertex3f(1.65, 0.65, 0.6);
  glVertex3f(1.65, 0.5, .6);
  glVertex3f(1.7, 0.5, 0.6);
  glVertex3f(1.7, 0.65, 0.6);
  glEnd();


  //**************************************************************
  glBegin(GL_QUADS);

  /* top of cube*/
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.6, 0.5, 0.2);       //quad front window
  glVertex3f(0.7, 0.65, 0.2);
  glVertex3f(0.7, 0.65, 0.6);

  glVertex3f(1.7, 0.65, .6);
  glVertex3f(1.7, 0.65, 0.2);      //quad back window
  glVertex3f(1.8, 0.5, 0.2);
  glVertex3f(1.8, 0.5, 0.6);
  glEnd();
  glBegin(GL_TRIANGLES);                /* start drawing the cube.*/
  /* top of cube*/
  glColor3f(0.3, 0.3, 0.3);
  glVertex3f(0.6, 0.5, 0.6);
  glVertex3f(0.7, 0.65, 0.6);      //tri front window
  glVertex3f(0.7, 0.5, 0.6);

  glVertex3f(0.6, 0.5, 0.2);
  glVertex3f(0.7, 0.65, 0.2);      //tri front window
  glVertex3f(0.7, 0.5, 0.2);

  glVertex3f(1.7, 0.65, 0.2);
  glVertex3f(1.8, 0.5, 0.2);      //tri back window
  glVertex3f(1.7, 0.5, 0.2);

  glVertex3f(1.7, 0.65, 0.6);
  glVertex3f(1.8, 0.5, 0.6);      //tri back window
  glVertex3f(1.7, 0.5, 0.6);
  glEnd();

  //************IGNITION SYSTEM
  glPushMatrix();
  glColor3f(0.3, 0.3, 0.7);
  glTranslatef(1.65, 0.2, 0.3);
  glRotatef(90.0, 0, 1, 0);
  gluCylinder(t, 0.02, 0.03, .5, 10, 10);
  glPopMatrix();

  //********************WHEEL
  glColor3f(0.7, 0.7, 0.7);
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  for(theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f(0.6, 0.2, 0.62);
    glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for(theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f(0.6, 0.2, 0.18);
    glVertex3f(0.6 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for(theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f(1.7, 0.2, 0.18);
    glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.18);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  for(theta = 0; theta < 360; theta = theta + 40) {
    glVertex3f(1.7, 0.2, 0.62);
    glVertex3f(1.7 + (0.08 * (cos(((theta + angle) * 3.14) / 180))), 0.2 + (0.08 * (sin(((theta + angle) * 3.14) / 180))), 0.62);
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.6, 0.2, 0.6);
  glColor3f(0, 0, 0);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(0, 0, -0.4);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(1.1, 0, 0);
  glutSolidTorus(0.025, 0.07, 10, 25);

  glTranslatef(0, 0, 0.4);
  glutSolidTorus(0.025, 0.07, 10, 25);
  glPopMatrix();

  /******************AANTENA**********************************/
  glPushMatrix();
  glColor3f(0.7, 0, 0.7);
  glTranslatef(2.0, 0.6, 0.4);
  glRotatef(cRot, 0.0f, 1.0f, 0.0f);
  glRotatef(90.0, 1, 0, 0);
  gluCylinder(t, 0.02, 0.03, .2, 10, 10);
  glTranslatef(0, 0, 0.15);
  glutSolidSphere(0.04, 10, 10);
  glTranslatef(0, -0.025, -0.15);
  glutSolidSphere(0.04, 10, 10);
  glTranslatef(0, 0.05, 0);
  glutSolidSphere(0.04, 10, 10);
  glPopMatrix();


  /*********************ROCKETS*****************************/
  glPushMatrix();
  glColor3f(0.3, 0.3, 0.7);
  glTranslatef(0.65, 0.4, 0.67);
  glRotatef(90.0, 0, 1, 0);
  gluCylinder(t, 0.06, 0.06, .2, 10, 10);
  gluCylinder(t, 0.06, 0, .2, 10, 10);
  glTranslated(0, 0, 0.19);
  gluCylinder(t, 0.06, 0, .2, 10, 10);

  glTranslatef(0.54, 0, -0.19);
  gluCylinder(t, 0.06, 0.06, .2, 10, 10);
  gluCylinder(t, 0.06, 0, .2, 10, 10);
  glTranslated(0, 0, 0.19);
  gluCylinder(t, 0.06, 0, .2, 10, 10);
  glPopMatrix();

  /********************REARLIGHTS***************************/
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(1.9, 0, 0);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.2, 0.3, 0.3);
  glVertex3f(0.2, 0.3, 0.5);
  glEnd();
  glPopMatrix();

  /*******************LICENSE*PLATE************************/
  char text[] = "CPSC 3710";
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glColor3f(0.2, 0.2, 0.2);
  glTexCoord2f(0.995f, 0.005f);
  glVertex3f(0.2f, 0.25f, 0.35f);
  glTexCoord2f(2.995f, 2.995f);
  glVertex3f(0.2f,  0.35f, 0.35f);
  glTexCoord2f(0.005f, 0.995f);
  glVertex3f(0.2f,  0.35f, 0.45f);
  glTexCoord2f(0.005f, 0.005f);
  glVertex3f(0.2f, 0.25f, 0.45f);
  glEnd();
}

void NormalKeyUp(GLubyte key, GLint x, GLint y) {
  switch(key) {
  case 'i': // forward
    forwardDown = false;
    break;
  case 'k': // back
    backDown = false;
  }
}

void NormalKey(GLubyte key, GLint x, GLint y) {
  switch(key)    {
  case ESCAPE :
    printf("escape pressed. exit.\n");
    glutDestroyWindow(window);
    exit(0);
    break;
  case 'i': // forward
    forwardDown = true;
    break;
  case 'k': // backward
    backDown = true;
    break;
  case 'j': // left
    turning = true;
    turnDir = 1;
    break;
  case 'l': // right
    turning = true;
    turnDir = -1;
    break;
  default:
    break;
  }
}

static void SpecialKeyFunc(int Key, int x, int y) {

}

void colorMenu(int id) {
  switch(id) {
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

int main(int argc, char **argv) {
  printf("Car Animation Demo for CPSC3710\n");
  printf("MOUSE\n");
  printf("\tPRESS RIGHT BUTTON FOR MENU\n");
  printf("KEYBOARD\n");
  printf("\tX-Y-Z KEYS FOR CORRESPONDING ROTATION\n");
  printf("\tU-F FOR CAMERA VIEW SETTINGS\n");
  printf("\tUSE LEFT ARROW(<-) AND RIGHT ARROW(->) TO MOVE CAR\n");
  printf("\tESCAPE TO EXIT\n");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(Xsize, Ysize);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("3D CAR ANIMATION");
  init();
  glutDisplayFunc(DrawGLScene);
  glutKeyboardFunc(NormalKey);
  glutKeyboardUpFunc(NormalKeyUp);
  glutSpecialFunc(SpecialKeyFunc);
  InitGL(Xsize, Ysize);

  glutCreateMenu(colorMenu);
  // You can change the color of the car
  glutAddMenuEntry("blue", 6);
  glutAddMenuEntry("red", 7);
  glutAddMenuEntry("green", 8);
  glutAddMenuEntry("black", 9);
  glutAddMenuEntry("yellow", 10);
  glutAddMenuEntry("grey", 11);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 1;
}
