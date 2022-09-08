/*-----------------------------------------------------------
 * An example program to draw a car
 *   Divide the window into four parts, each part shows an image
 *   resulting from a projection method.
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include<iostream>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#define  PI   3.141592653

#define Step  0.5

 /*-----Define a unit box--------*/
 /* Vertices of the box */
float  points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
float  heli_head[][3] = { {-0.5, -0.5, -0.5}, {0.15, -0.45, -0.15},
                      {0.15, -0.25, -0.15}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.15, -0.45, 0.15},
                      {0.15, -0.25, 0.15}, {-0.5, 0.5, 0.5} };

float  heli_tail[][3] = { {-0.15, -0.45, -0.15}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.15, 0.25, -0.15},
                      {-0.15, -0.45, 0.15}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.15, 0.25, 0.15} };

float  triangle[][3] = { {-0.5, 0.5, -0.0}, {0.15, -0.45, -0.15},
                      {0.15, 0.05, -0.15}, {-0.5, 0.5, -0.0},
                      {-0.5, 0.5, 0.0}, {0.15, -0.45, 0.15},
                      {0.15, 0.05, 0.15}, {-0.5, 0.5, 0.0} };
float Dodecahedron[20][3];//12面體
float Icosahedron[36][3];//20面體
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float  colors[6][3] = { {0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
                     {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7} };
/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;


/*-Declare car position, orientation--*/
float  self_ang = -90.0, glob_ang = 0.0, y_angle = 0.0, x_angle = 0.0;
float  position[3] = { 8.0, 0.0, 0.0 };
float bld_ang = 10.0;

/*-----Define window size----*/
int width = 600, height = 600;
double left = -60, right = 60, bottom = -60, top = 60;
double a = 1.0, ucenter, vcenter;
int las = 1;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */
/*----Define normals of faces ----*/
float  normal[][4] = { {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
              {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0} };

/*----Define material properties for cube -----*/
float  mat_ambient[] = { 0.1, 0.3, 0.1, 1.0 };
float  mat_diffuse[] = { 0.1, 0.9, 0.1, 1.0 };
float  mat_specular[] = { 0.4, 0.4, 0.4, 1.0 };
float  mat_shininess = 64.0;
float  flr_diffuse[] = { 0.60, 0.60, 0.60, 1.0 };
float  flr_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
float  flr_specular[] = { 0.0, 0.0, 0.0, 1.0 };

/*----Define light source properties -------*/
float  lit_position[] = { 10.0, 14.0, 0.0, 1.0 };
float  lit_direction[] = { -1.0, -1.0, 1.0, 0.0 };
float  lit_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
float  lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float  lit_cutoff = 60.0;
float  lit_exponent = 8.0;
/*----Define light source properties -------*/
float  lit2_position[] = { 0.0, 0.0, 0.0, 1.0 };
float  lit2_direction[] = { -1.0, 0.0, 0.0, 0.0 };
float  lit2_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
float  lit2_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float  lit2_cutoff = 30.0;
float  lit2_exponent = 8.0;
float  lit1_position[] = { 1.0, 1.0, 1.0, 0.0 };
float  lit1_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };
int light_switch = 0;

float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

/*----Some global variables for transformation------*/
float  lit_angle = 0.0, lit_angle2 = 0.0;
float  cube_angle = 0.0;
float  head = 0.0, roll = 0.0, Epitch = 0.0, Wpitch = 0.0;
float  upp = 0.0, rightt = 0.0, zoom = 0.0;

float expo = 20.0;
/*-----Drawing stye
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int style = 0;
int display_mod = 5;

void draw_scene(void);
void my_quit(unsigned char key, int x, int y);
float norm2(float v[]);
float norm[4] = { 0,0,0 };
void cross_product(float a[], float b[], float c[]) {//B在中間
    float u[3] = { c[0] - b[0],c[1] - b[1],c[2] - b[2] }, v[3] = { a[0] - b[0],a[1] - b[1],a[2] - b[2] };
    norm[0] = u[1] * v[2] - u[2] * v[1];
    norm[1] = u[2] * v[0] - u[0] * v[2];
    norm[2] = u[0] * v[1] - u[1] * v[0];
    glNormal3fv(norm);
    //glNormal3f(0, 1, 0);
}
unsigned int TextureID[4];
unsigned char circle[64][64][4];
unsigned char waterwave[64][64][4];
unsigned char star[64][64][4];
unsigned char land[64][64][4];
int texindex = 0;
#define TSIZE 64 /* define texture dimension */
/*---- the axes of billboard ----*/
float A[3], B[3];
/*-----Create image space for textures -----*/
unsigned char checkboard[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char brick[TSIZE][TSIZE][4];      /* brick wall textures */
unsigned int textName[2];                  /* declare two texture maps*/

float ds = 0.0, dt = 0.0;

float mtx[16]; /* the modelview matrix */
/*---Define fog color----*/
float fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
double texmov = 0;
int weather = 0;

void make_check() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            checkboard[i][j][0] = c / 8;
            checkboard[i][j][1] = c / 2;
            checkboard[i][j][2] = c / 4;
            if (c == 255)
                checkboard[i][j][3] = 255;
            else
                checkboard[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            checkboard[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            checkboard[i][j][3] = 0;
    }
}
void compute_ab_axes(void) {
    float w0, w2;
    double len;
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    B[0] = 0.0;
    B[1] = 1.0;
    B[2] = 0.0;
    A[0] = w2 / len;
    A[1] = 0.0;
    A[2] = -w0 / len;
}
/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * A[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * A[2];
    v1[0] = x + (w / 2) * A[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * A[2];
    v2[0] = x + (w / 2) * A[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * A[2];
    v3[0] = x - (w / 2) * A[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * A[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
    // glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
    glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);   /*Enable mornalization  */

    glEnable(GL_LIGHTING);    /*Enable lighting effects */
    glEnable(GL_LIGHT0);      /*Turn on light0 */
    glEnable(GL_LIGHT1);      /*Turn on light1 */
    /*-----Define light0 ---------*/
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);

    /*-------Draw light source as a red cube----*/
    glEnable(GL_LIGHT2);
    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lit2_exponent);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, expo);

    /*-----Define some global lighting status -----*/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    /*---Create quadratic objects---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    if (disk == NULL) {
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }

    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
    /*----Set up fog conditions ---*/
    glEnable(GL_FOG);                /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/

    //glFogi(GL_FOG_MODE, GL_EXP);
    //glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, 36.0);
    glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
}

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void myinit2() {
    //glClearColor(0.150, 0.150, 0.150, 1.0); /*set the background color BLACK */
                                            /*Clear the Depth & Color Buffers */
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_DEPTH_TEST); /*Enable depth buffer for shading computing */
    //glEnable(GL_NORMALIZE);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    //
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lit_ambient);
    //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    //glViewport(0, 0, 499, 499);

    /*-----Create Texture -----*/
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(2, textName);
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    /*-----Create another texture ----*/
    //brick_pattern();
    //glBindTexture(GL_TEXTURE_2D, textName[1]);
    //
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick);
}
/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */

void lt() {
    float  white[] = { 0.95, 0.95, 0.95, 1.0 };
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}
void cycn_rubber() {
    //Cyan rubber
    float mt_ambient[] = { 0.0f,0.05f,0.05f,1.0f };
    float mt_diffuse[] = { 0.4f,0.5f,0.5f,1.0f };
    float mt_specular[] = { 0.04f,0.7f,0.7f,1.0f };
    float  white[] = { 0.95, 0.95, 0.95, 1.0 };
    float shine = 10.0f;
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mt_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mt_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mt_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void gold() {
    //brass
    float mt_ambient[] = { 0.329412f, 0.223529f, 0.027451f,1.0f };
    float mt_diffuse[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
    float mt_specular[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
    float shine = 27.8974f;
    float  white[] = { 0.95, 0.95, 0.95, 1.0 };
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mt_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mt_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mt_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void sliver() {
    //Silver
    float mt_ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
    float mt_diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
    float mt_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
    float shine = 51.2f;
    float  white[] = { 0.95, 0.95, 0.95, 1.0 };
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3fv(white);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mt_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mt_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mt_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void draw_cube()
{
    float  range;
    /*-----Define some material properties shared by every one ---*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[texindex]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glRotatef(texmov, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    gold();

    for (int i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        range = 1.0;
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON);  /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(0.0, 2.0);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(2.0, 2.0);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(2.0, 0.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

void draw_head()
{
    gold();
    for (int i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(heli_head[face[i][0]]);
        glVertex3fv(heli_head[face[i][1]]);
        glVertex3fv(heli_head[face[i][2]]);
        glVertex3fv(heli_head[face[i][3]]);
        glEnd();
    }
}

void draw_tail_1() {
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[texindex]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    gold();
    for (int i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        glNormal3fv(normal[i]);

        glBegin(GL_POLYGON);  /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(heli_tail[face[i][0]]);
        glTexCoord2f(0.0, 2.0);
        glVertex3fv(heli_tail[face[i][1]]);
        glTexCoord2f(2.0, 2.0);
        glVertex3fv(heli_tail[face[i][2]]);
        glTexCoord2f(2.0, 0.0);
        glVertex3fv(heli_tail[face[i][3]]);
        glEnd();
    }
}

void draw_tail_2() {
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[texindex]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    gold();
    for (int i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        glNormal3fv(normal[i]);

        glBegin(GL_POLYGON);  /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(triangle[face[i][0]]);
        glTexCoord2f(0.0, 2.0);
        glVertex3fv(triangle[face[i][1]]);
        glTexCoord2f(2.0, 2.0);
        glVertex3fv(triangle[face[i][2]]);
        glTexCoord2f(2.0, 0.0);
        glVertex3fv(triangle[face[i][3]]);
        glEnd();
    }
}

void drawDodecahedron() {
    cycn_rubber();
    glColor3f(0, 127 / 255.0, 1);  /* Set color */
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Dodecahedron[0], Dodecahedron[1], Dodecahedron[2]);
    for (int i = 0; i < 10; i++) //上
        glVertex3fv(Dodecahedron[i]);
    glEnd();
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Dodecahedron[10], Dodecahedron[11], Dodecahedron[12]);
    for (int i = 10; i < 20; i++) //下
        glVertex3fv(Dodecahedron[i]);
    glEnd();
    //側邊
    for (int i = 0; i < 10; i++) {
        if (i % 2)
            glColor3f(100 / 255.0, 149 / 255.0, 237 / 255.0);
        else
            glColor3f(1, 1, 1);
        glBegin(GL_POLYGON);
        if (i != 9) {
            cross_product(Dodecahedron[i], Dodecahedron[i + 1], Dodecahedron[i + 11]);
            glVertex3fv(Dodecahedron[i]);
            glVertex3fv(Dodecahedron[i + 1]);
            glVertex3fv(Dodecahedron[i + 11]);
            glVertex3fv(Dodecahedron[i + 10]);
        }
        else {
            cross_product(Dodecahedron[0], Dodecahedron[i], Dodecahedron[i + 10]);
            glVertex3fv(Dodecahedron[0]);
            glVertex3fv(Dodecahedron[i]);
            glVertex3fv(Dodecahedron[i + 10]);
            glVertex3fv(Dodecahedron[10]);
        }
        glEnd();
    }
}
void drawDodecahedron_billboard() {
    cycn_rubber();
    glColor3f(0, 127 / 255.0, 1);  /* Set color */
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Dodecahedron[0], Dodecahedron[1], Dodecahedron[2]);
    for (int i = 0; i < 10; i++) //上
        glVertex3fv(Dodecahedron[i]);
    glEnd();
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Dodecahedron[10], Dodecahedron[11], Dodecahedron[12]);
    for (int i = 10; i < 20; i++) //下
        glVertex3fv(Dodecahedron[i]);
    glEnd();
    //側邊
    for (int i = 0; i < 10; i++) {
        if (i % 2)
            glColor3f(100 / 255.0, 149 / 255.0, 237 / 255.0);
        else
            glColor3f(1, 1, 1);
        glBegin(GL_POLYGON);
        if (i != 9) {
            cross_product(Dodecahedron[i], Dodecahedron[i + 1], Dodecahedron[i + 11]);
            glVertex3fv(Dodecahedron[i]);
            glVertex3fv(Dodecahedron[i + 1]);
            glVertex3fv(Dodecahedron[i + 11]);
            glVertex3fv(Dodecahedron[i + 10]);
        }
        else {
            cross_product(Dodecahedron[0], Dodecahedron[i], Dodecahedron[i + 10]);
            glVertex3fv(Dodecahedron[0]);
            glVertex3fv(Dodecahedron[i]);
            glVertex3fv(Dodecahedron[i + 10]);
            glVertex3fv(Dodecahedron[10]);
        }
        glEnd();
    }
}

void drawIcosahedron() {
    cycn_rubber();
    glColor3f(0, 127 / 255.0, 1);  /* Set color */
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Icosahedron[0], Icosahedron[1], Icosahedron[2]);
    for (int i = 0; i < 18; i++) //上
        glVertex3fv(Icosahedron[i]);
    glEnd();
    glBegin(GL_POLYGON);  /* Draw the face */
    cross_product(Icosahedron[18], Icosahedron[19], Icosahedron[20]);
    for (int i = 18; i < 36; i++) //下
        glVertex3fv(Icosahedron[i]);
    glEnd();
    //側邊
    for (int i = 0; i < 18; i++) {
        if (i % 2)
            glColor3f(100 / 255.0, 149 / 255.0, 237 / 255.0);
        else
            glColor3f(0, 51 / 255.0, 1);
        glBegin(GL_POLYGON);
        if (i != 17) {
            cross_product(Icosahedron[i], Icosahedron[i + 1], Icosahedron[i + 19]);
            glVertex3fv(Icosahedron[i]);
            glVertex3fv(Icosahedron[i + 1]);
            glVertex3fv(Icosahedron[i + 19]);
            glVertex3fv(Icosahedron[i + 18]);
        }
        else {
            cross_product(Icosahedron[0], Icosahedron[i], Icosahedron[i + 18]);
            glVertex3fv(Icosahedron[0]);
            glVertex3fv(Icosahedron[i]);
            glVertex3fv(Icosahedron[i + 18]);
            glVertex3fv(Icosahedron[18]);
        }
        glEnd();
    }
}

void draw_blade()
{
    sliver();
    //glNormal3fv(normal[2]);
    glBegin(GL_POLYGON);
    float q[] = { 0,0,0 }, qq[] = { 1,0,4 }, qqq[] = { 1,0,8 };
    cross_product(qqq, qq, q);
    glVertex3f(0.0, 0, 0.0);
    glVertex3f(1.0, 0, 4.0);
    glVertex3f(1.0, 0, 8.0);
    glVertex3f(-1.0, 0, 8.0);
    glVertex3f(-1.0, 0, 4.0);
    glEnd();
}

/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{

    lt();

    int  i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            if ((i + j) % 2 == 0) glColor3f(0.8, 0.8, 0.8);//1.0 0.8 0.8
            else glColor3f(0.2, 0.2, 0.4);//0.1 0.1 0.7
            glBegin(GL_POLYGON);
            float q[] = { (i - 5.0) * 10.0, -2.2, (j - 5.0) * 10.0 }, qq[] = { (i - 5.0) * 10.0, -2.2, (j - 4.0) * 10.0 }, qqq[] = { (i - 4.0) * 10.0, -2.2, (j - 4.0) * 10.0 };
            cross_product(q, qq, qqq);
            glVertex3f((i - 5.0) * 10.0, -2.2, (j - 5.0) * 10.0);
            glVertex3f((i - 5.0) * 10.0, -2.2, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.2, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.2, (j - 5.0) * 10.0);
            glEnd();                    //-2.5
        }
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{
    lt();
    /*----Draw a white sphere to represent the original-----*/
    glColor3f(0.9, 0.9, 0.9);

    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

/* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (x) {
    case 4:       /* Perspective */

      /* In this sample program, eye position and Xe, Ye, Ze are computed
         by ourselves. Therefore, use them directly; no transform is
         applied upon eye coordinate system
         */
        gluLookAt(eye[0], eye[1], eye[2],
            eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
            u[1][0], u[1][1], u[1][2]);
        break;

    case 1:       /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case 2:       /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        break;
    case 3:
        gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4) {
        gluPerspective(90.0, (double)width / (double)height, 1.5, 50.0);
    }
    else {
        /*if (width > height) {
            glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width,
                40.0 * (float)height / (float)width,
                -0.0, 100.0);
        }
        else {
            glOrtho(-40.0 * (float)width / (float)height,
                40.0 * (float)width / (float)height, -40.0, 40.0,
                -0.0, 100.0);
        }*/
        glOrtho(left, right, bottom, top, 0.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}


/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_vview(int n) {
    if (n == 1) {
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 40, -60);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, -40, -60);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, 30, -70);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, -30, -70);
        glEnd();
        glColor4f(1, 0, 0, 0.3);
        glBegin(GL_POLYGON);
        glVertex3f(-60, 30, -70);
        glVertex3f(-60, -30, -70);
        glVertex3f(60, -50, -60);
        glVertex3f(60, 50, -60);
        glEnd();
    }
    else if (n == 3) {
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(60, 60, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(60, -60, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, 60, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, -60, 0);
        glEnd();
        glColor4f(1, 0, 0, 0.3);
        glBegin(GL_POLYGON);
        glVertex3f(-60, -60, -60);
        glVertex3f(-60, 60, -60);
        glVertex3f(60, 60, -60);
        glVertex3f(60, -60, -60);
        glEnd();
    }
    else if (n == 2) {
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 40, -60);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(10, -40, -60);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, 30, -70);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-60, -30, -70);
        glEnd();
        glColor4f(1, 0, 0, 0.3);
        glBegin(GL_POLYGON);
        glVertex3f(-60, 30, -70);
        glVertex3f(-60, -30, -70);
        glVertex3f(60, -50, -60);
        glVertex3f(60, 50, -60);
        glEnd();
    }

}

void draw_view(int n)
{
    int    i;

    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    //draw_vview(n);
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();


    glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);

    glEnd();
}



/*--------------------------------------------------------
 * Display callback procedure to draw the scene
 */
void display()
{
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    compute_ab_axes();
    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glBindTexture(GL_TEXTURE_2D, textName[0]);
    
    glRotatef(texmov, 0, 0,0);

    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);

    //glDisable(GL_TEXTURE_2D);
    

    glLoadIdentity();

    glRotatef(head, 0.0, 1.0, 0.0);    /*--Transformation in Eye coord. sys--*/
    glRotatef(roll, 0.0, .0, 1.0);
    glRotatef(Epitch, 1.0, 0.0, 0.0);
    glTranslatef(0.0, upp, 0.0);
    glTranslatef(-right, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -zoom);

    glPushMatrix();
    glLoadIdentity();
    /*----position light1 (fixed) in eye coord sys ---*/
    glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);  /*fixed position in eye space---*/
    /*---draw the light source ---*/
    glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glColor3f(0.80, 0.80, 0.0);
    glutSolidSphere(0.3, 12, 12);
    glPopMatrix();
    glEnable(GL_CULL_FACE);
    /*----Define Viewing Matrix-----*/
    gluLookAt(8.0, 8.0, 20.0, 8.0, 4.0, 4.0, 0.0, 1.0, 0.0);



    /*-------Rotate light position----*/
    glPushMatrix();
    glTranslatef(58.0, 5.0, -18.0);          /* Move to [8, 0, 8] */
    glRotatef(lit_angle, 0.0, 1.0, 0.0);  /* Rotate about y-axis */
    /*-------Draw light source as a red cube----*/
    glPushMatrix();
    glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    glEnable(GL_LIGHTING);      /* Turn on lighting */
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    glPopMatrix();





    if (display_mod == 5) {
        switch (style) {
        case 0:
            make_view(4);
            make_projection(4);
            glViewport(width / 2, 0, width / 2, height / 2);
            draw_scene();
            make_view(1);
            make_projection(1);
            glViewport(0, height / 2, width / 2, height / 2);
            draw_scene();
            make_view(1);
            draw_view(1);

            make_view(2);
            glViewport(width / 2, height / 2, width / 2, height / 2);
            draw_scene();
            make_view(2);
            draw_view(2);

            make_view(3);
            glViewport(0, 0, width / 2, height / 2);
            draw_scene();
            make_view(3);
            draw_view(3);
            break;

        case 4:
            glViewport(0, 0, width, height);
            make_view(4);
            make_projection(4);
            draw_scene();
            break;
        }
    }
    else if (display_mod == 1) {
        make_projection(1);
        glViewport(0, 0, width, height);
        draw_scene();
        make_view(1);
        draw_view(1);
    }
    else if (display_mod == 2) {
        make_view(2);
        make_projection(1);
        glViewport(0, 0, width, height);
        draw_scene();
        make_view(2);
        draw_view(2);
    }
    else if (display_mod == 3) {
        make_view(3);
        make_projection(1);
        glViewport(0, 0, width, height);
        draw_scene();
        make_view(3);
        draw_view(3);
    }
    else if (display_mod == 4) {
        make_view(4);
        make_projection(4);
        glViewport(0, 0, width, height);
        draw_scene();
    }

    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
    return;

}
/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
double rotate_addSpeed = 0.0, rotate_speed = 0.5, move_speed = 0.05, move_addSpeed = 0.0;

void draw_building() {
    //雙子星
    //floor 1
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-35, 15, 0);
    glScalef(4, 4, 10);
    drawIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-20, 15, 0);
    glScalef(4, 4, 10);
    drawIcosahedron();
    glPopMatrix();
    //floor2
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-35, 15, 10);
    glScalef(3.2, 3.2, 8);
    drawIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-20, 15, 10);
    glScalef(3.2, 3.2, 8);
    drawIcosahedron();
    glPopMatrix();
    //floor3
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-35, 15, 18);
    glScalef(2.2, 2.2, 6);
    drawIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-20, 15, 18);
    glScalef(2.2, 2.2, 6);
    drawIcosahedron();
    glPopMatrix();
    //floor4
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-35, 15, 24);
    glScalef(1, 1, 3);
    drawIcosahedron();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-20, 15, 24);
    glScalef(1, 1, 3);
    drawIcosahedron();
    glPopMatrix();
    //floor5
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-35, 15, 27);
    glScalef(0.35, 0.35, 12.5);
    drawDodecahedron();
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-20, 15, 27);
    glScalef(0.35, 0.35, 12.5);
    drawDodecahedron();
    glPopMatrix();
    //bridge
    glPushMatrix();
    glRotatef(-90, 0, 1, 0);
    glTranslatef(-20, 12, 22);
    glScalef(1.5, 1.5, 12);
    drawDodecahedron();
    glPopMatrix();
}
/*-------------------------------------------------------
 * This procedure draw the car
 */
void draw_scene()
{

    draw_floor();

    draw_axes();

    draw_building();

    glTranslatef(position[0], position[1] + 10, position[2]);
    glRotatef(self_ang, 0.0, 1.0, 0.0);
    glRotatef(y_angle, 0, 0, 1);
    glRotatef(x_angle, 1, 0, 0);

    glPushMatrix();
    glTranslatef(0, 0, 5);
    glRotatef(lit_angle, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);
    glutWireSphere(0.5, 8, 8);
    glPopMatrix();

    glPushMatrix();              /* Save M1 coord. sys */
    glScalef(17.0, 6.0, 10.0);    /* Scale up the axes */
    draw_cube();
    glPopMatrix();               /* Get M1 back */

    //head
    glTranslatef(17, 0.0, 0);
    glRotatef(glob_ang, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(17.0, 6.0, 10.0);
    draw_head();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(0, -5, 0);
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.5, 8, 8);
    //glPopMatrix();

    //glEnable(GL_LIGHTING);      /* Turn on lighting */
    /*----Redefine position and direction of light0-----*/

    glPopMatrix();



    //tail-1
    glTranslatef(-34, 0.0, 0);
    glRotatef(glob_ang, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(17.0, 6.0, 10.0);
    draw_tail_1();
    glPopMatrix();

    //tail-2
    glTranslatef(-5, 1.0, 0);
    glRotatef(glob_ang, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(17.0, 9.0, 10.0);
    draw_tail_2();
    glPopMatrix();

    /*-------左支架------*/
    glColor3f(0.68, 0.68, 0.68);    /* Gray colored */


    glTranslatef(34.0, -8.0, -4.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 0.0, 1.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
    glScalef(1, 1, 28);                                     /*--- Draw a cylinder ---*/
    drawIcosahedron();

    glPopMatrix();         /* Get M2 back */

    /*-------右支架------*/
    glColor3f(0.68, 0.68, 0.68);    /* Gray colored */


    glTranslatef(0.0, 0.0, 8.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 0.0, 1.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
                                         /*--- Draw a cylinder ---*/
    glScalef(1, 1, 28);
    drawIcosahedron();
    glPopMatrix();         /* Get M2 back */


    /*-------右支架--連接1------*/
    glColor3f(0.53, 0.53, 0.53);     /* Gray colored */

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    glTranslatef(-8.0, 0.0, -2.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
                                         /*--- Draw a cylinder ---*/
    gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
        7.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();         /* Get M2 back */

    /*-------左支架--連接1------*/
    glColor3f(0.53, 0.53, 0.53);    /* Gray colored */

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    glTranslatef(0.0, 0.0, -4.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
                                         /*--- Draw a cylinder ---*/
    gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
        7.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();         /* Get M2 back */

    /*-------左支架--連接2------*/
    glColor3f(0.53, 0.53, 0.53);    /* Gray colored */

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    glTranslatef(-10.0, 0.0, 0.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
                                         /*--- Draw a cylinder ---*/
    gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
        7.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();         /* Get M2 back */

     /*-------右支架--連接2------*/
    glColor3f(0.53, 0.53, 0.53);    /* Gray colored */

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    glTranslatef(0.0, 0.0, 4.0);
    glPushMatrix();              /* Save M2 system */

    glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
                                         //and y become -z */
                                         /*--- Draw a cylinder ---*/
    gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
        7.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();         /* Get M2 back */

    //connect componment
    glTranslatef(6, 11, -2);
    glRotatef(glob_ang, 0.0, 1.0, 0.0);

    glPushMatrix();
    glScalef(2.0, 3.0, 2.0);
    draw_cube();
    glPopMatrix();

    /*------Draw three blades ---*/
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(bld_ang, 0.0, 1.0, 0.0);  /* Rotate about x axis, M5 coord. sys. */
    glScalef(2.0, 1.0, 2.5);
    draw_blade();/* draw the first blade */
    glPopMatrix();

    glPushMatrix();
    glRotatef(bld_ang + 120.0, 0.0, 1.0, 0.0);    /* rotate by 120 degree, M6 coord. sys */
    glScalef(2.0, 1.0, 2.5);
    draw_blade();/* draw 2nd blade */
    glPopMatrix();

    glPushMatrix();
    glRotatef(bld_ang + 240.0, 0.0, 1.0, 0.0);    /* rotate by 120 degree,M7 coord. sys */
    glScalef(2.0, 1.0, 2.5);
    draw_blade();/* draw 3rd blade */
    glPopMatrix();

    //tail connect componment
    glPushMatrix();
    glTranslatef(-25.2, -1.5, -1);

    glScalef(1.0, 1.0, 3);
    draw_cube();


    /*------Draw three blades ---*/
    glTranslatef(0, 0, -0.35);
    glColor3f(1.0, 1.0, 1.0);

    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(bld_ang, 0.0, 1.0, 0.0);  /* Rotate about x axis, M5 coord. sys. */

    glScalef(0.7, 0.7, 0.7);
    draw_blade();/* draw the first blade */

    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(120.0, 0.0, 1.0, 0.0);    /* rotate by 120 degree, M6 coord. sys */

    glScalef(1.0, 1.0, 1.0);
    draw_blade();/* draw 2nd blade */

    glRotatef(0, 1.0, 0.0, 0.0);
    glRotatef(120.0, 0.0, 1.0, 0.0);    /* rotate by 120 degree,M7 coord. sys */

    glScalef(1.0, 1.0, 1.0);
    draw_blade();/* draw 3rd blade */

    glPopMatrix();

}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (double)w / (double)h, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void real(unsigned char key, int x, int y) {
    if (key == 0x1B) exit(0);

    if (key == 'r' || key == 'R') {
        glutKeyboardFunc(my_quit);
    }
    else if (key == 'w' || key == 'W') {
        if (move_speed < 1) {
            move_speed += move_addSpeed;
            move_addSpeed += 0.001;
        }
        position[1] += move_speed;
    }
    else if (key == 's' || key == 'S') {
        if (position[1] > 0) {
            if (move_speed < 1) {
                move_speed += move_addSpeed;
                move_addSpeed += 0.001;
            }
            position[1] -= move_speed;
        }
    }
    else if (key == '8') {
        if (y_angle > -20) {
            if (rotate_speed < 4) {
                rotate_speed += rotate_addSpeed;
                rotate_addSpeed += 0.01;
            }
            y_angle -= rotate_speed;
        }
        if (move_speed < 0.5) {
            move_speed += move_addSpeed;
            move_addSpeed += 0.001;
        }
        position[2] -= move_speed * sin(self_ang * PI / 180.0);

    }
    else if (key == '5') {
        if (y_angle < 20) {
            if (rotate_speed < 4) {
                rotate_speed += rotate_addSpeed;
                rotate_addSpeed += 0.01;
            }
            y_angle += rotate_speed;
        }
        if (move_speed < 0.5) {
            move_speed += move_addSpeed;
            move_addSpeed += 0.001;
        }
        position[2] += move_speed * sin(self_ang * PI / 180.0);
    }
    else if (key == 'q' || key == 'Q') {
        if (rotate_speed < 10) {
            rotate_speed += rotate_addSpeed;
            rotate_addSpeed += 0.01;
        }
        self_ang += rotate_speed;
    }
    else if (key == 'e' || key == 'E') {
        if (rotate_speed < 10) {
            rotate_speed += rotate_addSpeed;
            rotate_addSpeed += 0.01;
        }
        self_ang -= rotate_speed;
    }
    else if (key == 'a' || key == 'A') {
        if (x_angle > -45) {
            if (rotate_speed < 4) {
                rotate_speed += rotate_addSpeed;
                rotate_addSpeed += 0.01;
            }
            x_angle -= rotate_speed;
        }
        if (move_speed < 0.5) {
            move_speed += move_addSpeed;
            move_addSpeed += 0.001;
        }
        position[0] -= move_speed * sin(self_ang * PI / 180.0);
    }
    else if (key == 'd' || key == 'D') {
        if (x_angle < 45) {
            if (rotate_speed < 4) {
                rotate_speed += rotate_addSpeed;
                rotate_addSpeed += 0.01;
            }
            x_angle += rotate_speed;
        }
        if (move_speed < 0.5) {
            move_speed += move_addSpeed;
            move_addSpeed += 0.001;
        }
        position[0] += move_speed * sin(self_ang * PI / 180.0);
    }
    else
        move_speed = 0.1, rotate_speed = 0.5, move_addSpeed = 0, y_angle = 0, rotate_addSpeed = 0.0, x_angle = 0.0;

    display();
}
/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
int sh = -1, colorful = -1, fogmod = 0,fogsw=0,fogsw2=0,colorcnt=0;
double fogstrong = 0.001;
void my_quit(unsigned char key, int ix, int iy)
{
    int    i;
    float  x[3], y[3], z[3];
    if (key == 0x1B) exit(0);
    else if (key == '\'')weather ^= 1;
    else if (key == '=')
        texindex = (texindex + 1) % 4;
    else if (key == '/') {
        if (sh == -1)
            sh = 1;
        else
            sh = -1;
    }
    else if (key == '?') {
        colorful *= -1;
    }
    else if (key == ':') {
        fogsw ^= 1;
    }
    else if (key == '\"') {
        fogsw2 ^= 1;
    }
    else if (key == ';') {
        fogmod = (fogmod + 1) % 3;
        if (fogmod == 0) {
            glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
            glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
            fog_color[0] = 0.15;
            fog_color[1] = 0.2;
            fog_color[2] = 0.2;
            fog_color[3] = 0.5;
            glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
            glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);
        }
        else if (fogmod == 1) {
            glFogi(GL_FOG_MODE, GL_EXP);
            glFogf(GL_FOG_DENSITY, 0.05);    /*fog opacity(density)= 0.25*/
            fog_color[0] = 0;
            fog_color[1] = 1;
            fog_color[2] = 0;
            fog_color[3] = 0.25;
            glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
            glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);
        }
        else {
            glFogi(GL_FOG_MODE, GL_EXP2);
            glFogf(GL_FOG_DENSITY, 0.025);    /*fog opacity(density)= 0.25*/
            fog_color[0] = 1;
            fog_color[1] = 0;
            fog_color[2] = 0;
            fog_color[3] = 0.1;
            glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
            glClearColor(fog_color[0], fog_color[1], fog_color[2], fog_color[3]);
        }
    }
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {

        if (key == '!') glEnable(GL_LIGHT0);
        else if (key == '@') glDisable(GL_LIGHT0);
        /*---Rotate light position by SPACE key ----*/
        else if (key == '#') lit_angle += 5.0;
        else if (key == '$')glEnable(GL_LIGHT1);
        else if (key == '%')glDisable(GL_LIGHT1);
        else if (key == '^')glEnable(GL_LIGHT2);
        else if (key == '&')glDisable(GL_LIGHT2);
        else if (key == '*') {
            //std::cout << light_switch;
            light_switch ^= 1;
            if (light_switch) {
                lit2_diffuse[0] = 0;
                lit2_diffuse[1] = 1;
                lit2_diffuse[2] = 0;
                glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
                lit1_diffuse[0] = 0;
                lit1_diffuse[1] = 1;
                lit1_diffuse[2] = 0;
                glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
                lit_diffuse[0] = 0;
                lit_diffuse[1] = 1;
                lit_diffuse[2] = 0;
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            }
            else {
                lit2_diffuse[0] = 0.95;
                lit2_diffuse[1] = 0.95;
                lit2_diffuse[2] = 0.95;
                glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
                lit1_diffuse[0] = 0.95;
                lit1_diffuse[1] = 0.95;
                lit1_diffuse[2] = 0.95;
                glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
                lit_diffuse[0] = 0.95;
                lit_diffuse[1] = 0.95;
                lit_diffuse[2] = 0.95;
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            }
        }
        else if (key == '(') {
            lit2_diffuse[0] += 0.1;
            lit2_diffuse[1] += 0.1;
            lit2_diffuse[2] += 0.1;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] += 0.1;
            lit1_diffuse[1] += 0.1;
            lit1_diffuse[2] += 0.1;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] += 0.1;
            lit_diffuse[1] += 0.1;
            lit_diffuse[2] += 0.1;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (key == ')') {
            lit2_diffuse[0] -= 0.1;
            lit2_diffuse[1] -= 0.1;
            lit2_diffuse[2] -= 0.1;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] -= 0.1;
            lit1_diffuse[1] -= 0.1;
            lit1_diffuse[2] -= 0.1;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] -= 0.1;
            lit_diffuse[1] -= 0.1;
            lit_diffuse[2] -= 0.1;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (key == '<') {
            lit2_cutoff -= 5;
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
            //std::cout << lit2_cutoff << "\n";
        }
        else if (key == '>') {
            lit2_cutoff += 5;
            if (lit2_cutoff > 61)
                lit2_cutoff -= 5;
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit2_cutoff);
        }
        else if (key == '{') {
            expo += 10;
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, expo);
        }
        else if (key == '}') {
            expo -= 10;
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, expo);
        }

    }
    else {
        if (key == 'r' || key == 'R') {
            glutKeyboardFunc(real);
        }
        else if (key == 'w') {
            position[1] += 1;
        }
        else if (key == 's') {
            if (position[1] != 0)
                position[1] -= 1;
        }
        else if (key == 'W') {
            y_angle -= 10;
        }
        else if (key == 'S') {
            y_angle += 10;
        }
        else if (key == '8') {                              /* move car ahead, SPACE key */
            position[0] += Step * cos(self_ang * PI / 180.0);
            position[2] -= Step * sin(self_ang * PI / 180.0);
        }
        else if (key == '5') {                        /* move car back b-key */
            position[0] -= Step * cos(self_ang * PI / 180.0);
            position[2] += Step * sin(self_ang * PI / 180.0);
        }
        else if (key == 'Q' || key == 'q') {                        /* make a left turn */
            self_ang += 10.0;
            if (self_ang > 360.0) self_ang -= 360.0;
        }
        else if (key == 'e' || key == 'E') {                        /* make a right turn */
            self_ang += -10.0;
            if (self_ang < 0.0) self_ang += 360.0;

            /*------transform the EYE coordinate system ------*/
        }
        else if (key == 'k' || key == 'K') {
            eyeDy += 0.5;       /* move up */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
        }
        else if (key == 'i' || key == 'I') {
            eyeDy += -0.5;       /* move down */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
        }
        else if (key == 'l' || key == 'L') {
            eyeDx += -0.5;       /* move left */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
        }
        else if (key == 'j' || key == 'J') {
            eyeDx += 0.5;        /* move right */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
        }
        else if (key == '+') {
            //std::cout << eye[0] << " " << eye[1] << " " << eye[2] << "\n";
            eyeDz += 0.5;        /* zoom in */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[2][i];
            ucenter = (left + right) / 2.0;
            vcenter = (bottom + top) / 2.0;
            left = left - ucenter;
            right = right - ucenter;
            bottom = bottom - vcenter;
            top = top - vcenter;

            if (las == 0) {
                las = 1;
                a = 1.0;
            }
            a -= 0.005;
            left = a * left;
            right = a * right;
            bottom = a * bottom;
            top = a * top;

            left = left + ucenter;
            right = right + ucenter;
            bottom = bottom + vcenter;
            top = top + vcenter;
            //std::cout << left << " " << right << " " << bottom << " " << top << "\n";
        }
        else if (key == '-') {
            eyeDz += -0.5;       /* zoom out */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[2][i];
            left = left + ucenter;
            right = right + ucenter;
            bottom = bottom + vcenter;
            top = top + vcenter;

            if (las == 1) {
                las = 0;
                a = 1.0;
            }
            a += 0.005;
            left = a * left;
            right = a * right;
            bottom = a * bottom;
            top = a * top;

            ucenter = (left + right) / 2.0;
            vcenter = (bottom + top) / 2.0;
            left = left - ucenter;
            right = right - ucenter;
            bottom = bottom - vcenter;
            top = top - vcenter;




            // std::cout << left << " " << right << " " << bottom << " " << top << "\n";
        }
        else if (key == 'p') {             /* pitching */
            eyeAngx += 5.0;
            if (eyeAngx > 360.0) eyeAngx -= 360.0;
            y[0] = u[1][0] * cv - u[2][0] * sv;
            y[1] = u[1][1] * cv - u[2][1] * sv;
            y[2] = u[1][2] * cv - u[2][2] * sv;

            z[0] = u[2][0] * cv + u[1][0] * sv;
            z[1] = u[2][1] * cv + u[1][1] * sv;
            z[2] = u[2][2] * cv + u[1][2] * sv;

            for (i = 0; i < 3; i++) {
                u[1][i] = y[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'P') {
            eyeAngx += -5.0;
            if (eyeAngx < 0.0) eyeAngx += 360.0;
            y[0] = u[1][0] * cv + u[2][0] * sv;
            y[1] = u[1][1] * cv + u[2][1] * sv;
            y[2] = u[1][2] * cv + u[2][2] * sv;

            z[0] = u[2][0] * cv - u[1][0] * sv;
            z[1] = u[2][1] * cv - u[1][1] * sv;
            z[2] = u[2][2] * cv - u[1][2] * sv;

            for (i = 0; i < 3; i++) {
                u[1][i] = y[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'h') {            /* heading */
            eyeAngy += 5.0;
            if (eyeAngy > 360.0) eyeAngy -= 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] - sv * u[2][i];
                z[i] = sv * u[0][i] + cv * u[2][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'H') {
            eyeAngy += -5.0;
            if (eyeAngy < 0.0) eyeAngy += 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] + sv * u[2][i];
                z[i] = -sv * u[0][i] + cv * u[2][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 't') {            /* rolling */
            eyeAngz += 5.0;
            if (eyeAngz > 360.0) eyeAngz -= 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] - sv * u[1][i];
                y[i] = sv * u[0][i] + cv * u[1][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[1][i] = y[i];
            }
        }
        else if (key == 'T') {
            eyeAngz += -5.0;
            if (eyeAngz < 0.0) eyeAngz += 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] + sv * u[1][i];
                y[i] = -sv * u[0][i] + cv * u[1][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[1][i] = y[i];
            }
        }
        else if (key == '0') {//switch
            display_mod = 1;
        }
        else if (key == '1') {
            display_mod = 2;
        }
        else if (key == '2') {
            display_mod = 3;
        }
        else if (key == '3') {
            display_mod = 4;
        }
        else if (key == '4') {
            display_mod = 5;
        }
    }
    display();
}

/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
    if (weather) {
        texmov += 0.5;
        if (texmov > 360.0) {
            texmov -= 360.0;
        }
    }
    bld_ang += 50.0;
    while (bld_ang > 360.0) {
        bld_ang -= 360.0;
    }
    if (fogsw) {
        glFogf(GL_FOG_DENSITY, fogstrong);
        fogstrong += 0.0002;
        if (fogstrong > 0.5)
            fogstrong = 0;
    }
    if (fogsw2) {
        float ccolor[] = { 0,0,0,1 };
        if (colorcnt < 500) {
            ccolor[0] = colorcnt  / 500.0;
            ccolor[1] = colorcnt  / 500.0;
            ccolor[2] = colorcnt  / 500.0;
        }
        else if (colorcnt < 1000) {
            ccolor[0] = (1000-colorcnt)  / 500.0;
            ccolor[1] = (1000 - colorcnt)  / 500.0;
            ccolor[2] = (1000 - colorcnt)  / 500.0;
        }
        else {
            colorcnt = 0;
        }
        colorcnt++;
        glClearColor(ccolor[0], ccolor[1], ccolor[2], ccolor[3]);
        glFogfv(GL_FOG_COLOR, ccolor);
    }
    if (sh != -1) {
        sh ^= 1;
        if (sh == 1) {
            lit2_diffuse[0] += 1.0;
            lit2_diffuse[1] += 1.0;
            lit2_diffuse[2] += 1.0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] += 1.0;
            lit1_diffuse[1] += 1.0;
            lit1_diffuse[2] += 1.0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] += 1.0;
            lit_diffuse[1] += 1.0;
            lit_diffuse[2] += 1.0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else {
            lit2_diffuse[0] -= 1.0;
            lit2_diffuse[1] -= 1.0;
            lit2_diffuse[2] -= 1.0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] -= 1.0;
            lit1_diffuse[1] -= 1.0;
            lit1_diffuse[2] -= 1.0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] -= 1.0;
            lit_diffuse[1] -= 1.0;
            lit_diffuse[2] -= 1.0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
    }
    else if (colorful != -1) {
        colorful = (colorful + 1) % 7 + 1;
        if (colorful == 1) {
            lit2_diffuse[0] = 1;
            lit2_diffuse[1] = 0;
            lit2_diffuse[2] = 0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 1;
            lit1_diffuse[1] = 0;
            lit1_diffuse[2] = 0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 1;
            lit_diffuse[1] = 0;
            lit_diffuse[2] = 0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 2) {
            lit2_diffuse[0] = 1;
            lit2_diffuse[1] = 165 / 255.0;
            lit2_diffuse[2] = 0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 1;
            lit1_diffuse[1] = 165 / 255.0;
            lit1_diffuse[2] = 0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 1;
            lit_diffuse[1] = 165 / 255.0;
            lit_diffuse[2] = 0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 3) {
            lit2_diffuse[0] = 1;
            lit2_diffuse[1] = 1;
            lit2_diffuse[2] = 0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 1;
            lit1_diffuse[1] = 1;
            lit1_diffuse[2] = 0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 1;
            lit_diffuse[1] = 1;
            lit_diffuse[2] = 0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 4) {
            lit2_diffuse[0] = 0;
            lit2_diffuse[1] = 1;
            lit2_diffuse[2] = 0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 0;
            lit1_diffuse[1] = 1;
            lit1_diffuse[2] = 0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 0;
            lit_diffuse[1] = 1;
            lit_diffuse[2] = 0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 5) {
            lit2_diffuse[0] = 0;
            lit2_diffuse[1] = 0;
            lit2_diffuse[2] = 1;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 0;
            lit1_diffuse[1] = 0;
            lit1_diffuse[2] = 1;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 0;
            lit_diffuse[1] = 0;
            lit_diffuse[2] = 1;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 6) {
            lit2_diffuse[0] = 0;
            lit2_diffuse[1] = 0;
            lit2_diffuse[2] = 139 / 255.0;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 0;
            lit1_diffuse[1] = 0;
            lit1_diffuse[2] = 139 / 255.0;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 0;
            lit_diffuse[1] = 0;
            lit_diffuse[2] = 139 / 255.0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
        else if (colorful == 7) {
            lit2_diffuse[0] = 155 / 255.0;
            lit2_diffuse[1] = 48 / 255.0;
            lit2_diffuse[2] = 1;
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
            lit1_diffuse[0] = 155 / 255.0;
            lit1_diffuse[1] = 48 / 255.0;
            lit1_diffuse[2] = 1;
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            lit_diffuse[0] = 155 / 255.0;
            lit_diffuse[1] = 48 / 255.0;
            lit_diffuse[2] = 1;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        }
    }
    display(); /* show the scene again */
}
void Create_Texture_Circle() {
    int y, x, i, temp;
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            circle[y][x][0] = 100;
            circle[y][x][1] = 100;
            circle[y][x][2] = 100;
            circle[y][x][3] = 255;
        }
    }
    for (y = 16, x = 0; y < 48; y++) {
        if (y <= 20 && y >= 17)
            x = x + 2;
        else if (y == 22 || y == 25 || y == 31)
            x = x + 1;
        else if (y == 42 || y == 39 || y == 33)
            x = x - 1;
        else if (y <= 47 && y >= 44)
            x = x - 2;
        temp = x;
        for (x = 0; x < temp; x++) {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        for (x = 64 - temp; x < 64; x++) {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        x = temp;
    }

    for (x = 16, y = 0; x < 48; x++) {
        if (x <= 20 && x >= 17)
            y = y + 2;
        else if (x == 22 || x == 25 || x == 31)
            y = y + 1;
        else if (x == 42 || x == 39 || x == 33)
            y = y - 1;
        else if (x <= 47 && x >= 44)
            y = y - 2;
        temp = y;
        for (y = 0; y < temp; y++) {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        for (y = 64 - temp; y < 64; y++) {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        y = temp;
    }
    for (i = 0; i < 61; i++) {
        for (y = i; y < 4 + i; y++) {
            for (x = i; x < 4 + i; x++) {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
    for (i = 0; i < 61; i++) {
        for (y = i; y < 4 + i; y++) {
            for (x = 60 - i; x < 64 - i; x++) {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
}

void Create_Texture_Waterwave() {
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 32; y++) {
            waterwave[x][y][0] = 0;
            waterwave[x][y][1] = 100;
            waterwave[x][y][2] = 255;
            waterwave[x][y][3] = 255;
        }

    x = 0;
    y = 4;
    while (y <= 28) {
        for (i = x; i < 32; i++) {
            waterwave[i][y][0] = 200;
            waterwave[i][y][1] = 255;
            waterwave[i][y][2] = 255;
            waterwave[i][y][3] = 255;
        }
        y++;
        if (y < 9)
            x += 3;
        else if (y == 9)
            x++;
        else if (y == 10)
            x += 2;
        else if (y == 11)
            x++;
        else if (y == 12)
            x += 2;
        else if (y == 13)
            x++;
        else if (y == 14)
            x += 2;
        else if (y == 16)
            x++;
        else if (y == 19)
            x++;
        else if (y == 21)
            x++;
        else if (y == 23)
            x++;
        else if (y == 26)
            x++;
        else
            x++;
    }
    x = 0;
    y = 0;
    while (y <= 24) {
        for (i = x; i < 32; i++) {
            waterwave[i][y][0] = 0;
            waterwave[i][y][1] = 100;
            waterwave[i][y][2] = 255;
            waterwave[i][y][3] = 255;
        }
        y++;
        if (y < 5)
            x += 3;
        else if (y == 5)
            x++;
        else if (y == 6)
            x += 2;
        else if (y == 7)
            x++;
        else if (y == 8)
            x += 2;
        else if (y == 9)
            x++;
        else if (y == 10)
            x += 2;
        else if (y == 11)
            x++;
        else if (y == 14)
            x++;
        else if (y == 16)
            x++;
        else if (y == 18)
            x++;
        else if (y == 21)
            x++;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 32; y++)
            for (i = 0; i < 4; i++)
                waterwave[x][y][i] = waterwave[63 - x][y][i];

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++) {
                if (x >= 47)
                    waterwave[x][y][i] = waterwave[x - 47][y - 32][i];
                else
                    waterwave[x][y][i] = waterwave[x + 16][y - 32][i];
            }
}

void Create_Texture_Land() {
    int x, y, i;

    for (y = 0; y < 32; y++)
        for (x = 0; x < 32; x++) {
            land[x][y][0] = 240;
            land[x][y][1] = 240;
            land[x][y][2] = 240;
            land[x][y][3] = 255;
        }

    for (x = 24; x < 32; x++) {
        land[x][0][0] = 255;
        land[x][0][1] = 200;
        land[x][0][2] = 200;
        land[x][0][3] = 255;

        land[0][x][0] = 255;
        land[0][x][1] = 200;
        land[0][x][2] = 200;
        land[0][x][3] = 255;
    }
    for (x = 26; x < 32; x++) {
        land[x][1][0] = 255;
        land[x][1][1] = 200;
        land[x][1][2] = 200;
        land[x][1][3] = 255;

        land[1][x][0] = 255;
        land[1][x][1] = 200;
        land[1][x][2] = 200;
        land[1][x][3] = 255;
    }
    for (x = 27; x < 32; x++) {
        for (y = 2; y < 4; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 28; x < 32; x++) {
        for (y = 4; y < 7; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 29; x < 32; x++) {
        for (y = 7; y < 9; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 30; x < 32; x++) {
        for (y = 9; y < 12; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 31; x < 32; x++) {
        for (y = 12; y < 16; y++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }

    for (y = 16; y < 32; y++)
        for (x = 16; x < 32; x++) {
            land[x][y][0] = 200;
            land[x][y][1] = 160;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }
    for (y = 20; y < 32; y++)
        for (x = 20; x < 32; x++) {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }

    for (x = 63; x > 31; x--) {
        for (y = 0; y < 32; y++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[63 - x][y][i];
    }

    /* Create the rest half part */
    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[x][63 - y][i];
}

void Create_Texture_Star() {
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++) {
            star[x][y][0] = 255;
            star[x][y][1] = 255;
            star[x][y][2] = 255;
            star[x][y][3] = 255;
        }

    x = 31;
    y = 60;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 100;
            star[x][i][1] = 100;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 3;
        x--;
    }
    x = 31;
    y = 20;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = 255;
            star[x][i][1] = 255;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 1;
        x--;
    }
    x = 4;
    y = 40;
    while (y >= 18) {
        for (i = x; i < 32; i++) {
            star[i][y][0] = 100;
            star[i][y][1] = 100;
            star[i][y][2] = 255;
            star[i][y][3] = 255;
        }
        y -= 1;
        if ((y % 2) == 1)
            x += 2;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; i++)
                star[x][y][i] = star[63 - x][y][i];
}

void Initial() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(4, TextureID);

    Create_Texture_Circle();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle);

    Create_Texture_Star();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, star);

    Create_Texture_Land();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, land);

    Create_Texture_Waterwave();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, waterwave);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lit_attenuation);
    //glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3f(1.0, 1.0, 1.0);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    printf("Press space to change texture map\n");
    printf("Press q to exit program\n");
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    //產生正10邊形的點
    int n = 10;
    float x = 0.0, y = 1.0, theta = PI / (n / 2.0);
    for (int i = 0; i < n * 2; i++) {
        Dodecahedron[i][0] = cos(theta * i) * x - sin(theta * i) * y;
        Dodecahedron[i][1] = sin(theta * i) * x + cos(theta * i) * y;
        Dodecahedron[i][2] = i < n;
        //std::cout << Dodecahedron[i][0] << " " << Dodecahedron[i][1] << " " << Dodecahedron[i][0] << "\n";
    }
    n = 18, theta = PI / (n / 2.0);//正18邊形
    for (int i = 0; i < n * 2; i++) {
        Icosahedron[i][0] = cos(theta * i) * x - sin(theta * i) * y;
        Icosahedron[i][1] = sin(theta * i) * x + cos(theta * i) * y;
        Icosahedron[i][2] = i < n;
    }
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("windmill");

    myinit();      /*---Initialize other state varibales----*/
    Initial();
    myinit2();
    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    /*  glutIdleFunc(display); */
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_quit);
    glutIdleFunc(idle_func);

    glutMainLoop();
}


/*------------------------------------------
 * Procedure to compute norm of vector v[]
 */
float norm2(float v[])
{
    double  sum;
    float   temp;

    sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    temp = (float)sqrt(sum);
    return temp;
}