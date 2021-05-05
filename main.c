#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include "load.h"
#include "transform.h"
#include "draw.h"
#include "texture.h"

//=========================================================================================================================================
//vektor struktúra
typedef struct vec3
{
    float x;
    float y;
    float z;
} vec3;

//kamera struktúra
typedef struct Camera
{
    vec3 position;
    vec3 rotation;
    vec3 speed;
    float distance;
} Camera;

typedef struct Cursor
{
    vec3 position;
    vec3 movement;
}Cursor;

//=========================================================================================================================================
//kamera
Camera camera; //stuktúra + változónév
//kurzor
Cursor cursor;
//fény változók
GLfloat lightPosition[] = {1, 1, 1, 0.0};
GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};
GLfloat globalAmbientLight[] = {0.8, 0.8, 0.8, 1.0};
bool lightOn = true;
//modellek
Model meat;
Model pan;
Model stove;
Model refrigerator;
Model table;
Model lightSwitch;
//textúrák
GLuint tex_meat;
GLuint tex_pan;
GLuint tex_stove;
GLuint tex_refrigerator;
GLuint tex_table;
GLuint tex_lightSwitch;
GLuint tex_floor;
GLuint tex_wall;
GLuint tex_info;
//hús pozíciója
vec3 meat_position;
float meat_speed = 0.05;
//info ablak változó
bool showInfo = false;

//=========================================================================================================================================
void loadModels();
void loadTextures();
void renderScene();
void reshape();
void mouseMotion();
void mouse();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void idle();
void drawTexturedRectangle(GLuint texture);
void setOrthographicProjection();
void restorePerspectiveProjection();
float distance(float x1, float x2, float y1, float y2);

//=========================================================================================================================================
int main(int argc, char* argv[])
{
    //GLUT inicializálás
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(1280,720);
	int window = glutCreateWindow("Ablak");
	glutSetWindow(window);

	//OpenGL inicializálás
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

	glClearColor(0.8, 0.8, 0.8, 0.0);
    glClearDepth(1.0);

    //modellek betöltése
    loadModels();

    //textúrák betöltése
    loadTextures();

    //hús kezdőpozíció
    meat_position.x = 0;
    meat_position.y = 0;
    meat_position.z = 0;

    //kamera beállítása
    camera.position.z = 10;
    camera.rotation.z = 90;
    camera.distance = 10;
    camera.speed.z = 0.01;

    //fény beállítása
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

	//render
	glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMotion);
    //glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();

    return 0;
}

//=========================================================================================================================================
void loadModels()
{
    load_model(&meat, "models/raw_meat.obj");
    scale_model(&meat, 0.5, 0.5, 0.5);
    load_model(&pan, "models/frying_pan.obj");
    load_model(&stove, "models/stove.obj");
    scale_model(&stove, 0.2, 0.2, 0.2);
    load_model(&refrigerator, "models/refrigerator.obj");
    scale_model(&refrigerator, 0.4, 0.4, 0.4);
    load_model(&table, "models/table.obj");
    scale_model(&table, 0.2, 0.2, 0.2);
    load_model(&lightSwitch, "models/light_switch.obj");
    scale_model(&lightSwitch, 0.25, 0.25, 0.25);
}

void loadTextures()
{
    tex_meat = load_texture("textures/raw_meat.png");
    tex_pan = load_texture("textures/frying_pan.jpg");
    tex_stove = load_texture("textures/stove.png");
    tex_refrigerator = load_texture("textures/refrigerator.jpg");
    tex_table = load_texture("textures/table.png");
    tex_lightSwitch = load_texture("textures/light_switch.png");
    tex_floor = load_texture("textures/tile_floor.jpg");
    tex_wall = load_texture("textures/wall.png");
    tex_info = load_texture("textures/info.png");
}

void renderScene()
{
    //buffer ürítés
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//render
	glPushMatrix();
        //kamera pozícionálása
        camera.position.x = camera.distance * cos(camera.rotation.z);
        camera.position.y = camera.distance * sin(camera.rotation.z);

        gluLookAt(
                  camera.position.x, camera.position.y, camera.position.z,  //nézõpont
                  0, 0, 0,                                                  //nézett pont
                  0.0, 0.0, 1.0                                             //fel iránya (ahol az 1-es van, az a felfelfe)
                  );
        //fény pozícionálása
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);   //újra meghívva, hogy a fény egy helyen maradjon

        //hús kirajzoláas
        glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, tex_meat);
            glTranslatef(meat_position.x, meat_position.y, meat_position.z);
            draw_model(&meat);
        glPopMatrix();

        //serpenyő kirajzolása
        glBindTexture(GL_TEXTURE_2D, tex_pan);
        draw_model(&pan);

        //sütő kirajzolása
        glPushMatrix();
            glTranslatef(5, -1, 0);//hova rajzolja ki
            glRotatef(90, 0, 0, 1);//forgatás
            glBindTexture(GL_TEXTURE_2D, tex_stove);
            draw_model(&stove);
        glPopMatrix();

        //hűtő kirajzolása
        glPushMatrix();
            glTranslatef(-30, 0, -18);
            glRotatef(-90, 0, 0, 1);
            glBindTexture(GL_TEXTURE_2D, tex_refrigerator);
            draw_model(&refrigerator);
        glPopMatrix();

        //asztal kirajzolása
        glPushMatrix();
            glTranslatef(-40, 50, -18);
            glBindTexture(GL_TEXTURE_2D, tex_table);
            draw_model(&table);
        glPopMatrix();

        //padló kirajzolása
        glPushMatrix();
            glTranslatef(0, 30, -18);
            glScalef(100.0, 100.0, 100.0);
            drawTexturedRectangle(tex_floor);
        glPopMatrix();

        //fal kirajzolása
        glPushMatrix();
            glTranslatef(0, -9, 2);
            glRotatef(-90, 1, 0, 0);
            glScalef(100.0, 100.0, 100.0);
            drawTexturedRectangle(tex_wall);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(50, 30, 2);
            glRotatef(90, 0, 1, 0);
            glScalef(100.0, 100.0, 100.0);
            drawTexturedRectangle(tex_wall);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-50, 30, 2);
            glRotatef(90, 0, 1, 0);
            glScalef(100.0, 100.0, 100.0);
            drawTexturedRectangle(tex_wall);
        glPopMatrix();

        //villanykapcsoló kirajzolása
        glPushMatrix();
            glTranslatef(30, -9, 5);
            glRotatef(90, 0, 0, 1);
            if(!lightOn)
            {
                glRotatef(180, 1, 0, 0);
            }
            glBindTexture(GL_TEXTURE_2D, tex_lightSwitch);
            draw_model(&lightSwitch);
        glPopMatrix();
    glPopMatrix();

    //Info ablak mutatása
    if(showInfo)
    {
        //2D nézet, árnyalás ki
        setOrthographicProjection();
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

        //info kirajzolása
        glPushMatrix();
            glLoadIdentity();
            glBindTexture(GL_TEXTURE_2D, tex_info);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex3d(0, 0, 0);
                glTexCoord2f(0.0, 1.0);
                glVertex3d(0, 720, 0);
                glTexCoord2f(1.0, 1.0);
                glVertex3d(1280, 720, 0);
                glTexCoord2f(1.0, 0.0);
                glVertex3d(1280, 0, 0);
            glEnd();
        glPopMatrix();

        //perspektíva visszaállítása, árnyalás be
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
        restorePerspectiveProjection();
    }

	//képcsere
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height)
{
	glViewport (0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.01, 200.0);
}

void mouseMotion(int x, int y)
{
    //kurzor mozgása az előző pozícióhóz képest
    cursor.movement.x = cursor.position.x - x;
    cursor.movement.y = cursor.position.y - y;

    //kamera forgatása a mozgástól függően
    camera.rotation.z += cursor.movement.x * camera.speed.z;

    //kurzor pozíciójának mentése
    cursor.position.x = x;
    cursor.position.y = y;
}

void mouse(int button, int state, int x, int y)
{
    //görgőzés
    if ((button == 3) || (button == 4)) // 3 = fel, 4 = le
    {
        if(button == 3)
        {
            if(camera.distance > 1)
                camera.distance--;
        }
        else
        {
            camera.distance++;
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
        if(distance(meat_position.x - meat_speed, 0, meat_position.y, 0) < 1.5)
        {
            meat_position.x -= meat_speed;
        }
        break;
    case 'a':
        if(distance(meat_position.x + meat_speed, 0, meat_position.y, 0) < 1.5)
        {
            meat_position.x += meat_speed;
        }
        break;
    case 'w':
        if(distance(meat_position.x, 0, meat_position.y - meat_speed, 0) < 1.5)
        {
            meat_position.y -= meat_speed;
        }
        break;
    case 's':
        if(distance(meat_position.x, 0, meat_position.y + meat_speed, 0) < 1.5)
        {
            meat_position.y += meat_speed;
        }
        break;
    case 'l':
        if(lightOn)
        {
            glDisable(GL_LIGHT0);
            lightOn = false;
        }
        else if(!lightOn)
        {
            glEnable(GL_LIGHT0);
            lightOn = true;
        }
        break;
    }
}

void specialKeys(int key, int x, int y)
{
	//info ablak váltás
	if (key == GLUT_KEY_F1)
	{
		if(showInfo)
        {
            showInfo = false;
        }
        else
        {
            showInfo = true;
        }
	}
}

void drawTexturedRectangle(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0.0,0.0); //bal felső sarok
        glVertex3f( -0.5f, -0.5f, 0);
        glTexCoord2f(1.0,0.0); //bal alsó sarok
        glVertex3f( 0.5f, -0.5f, 0);
        glTexCoord2f(1.0,1.0); //jobb alsó
        glVertex3f( 0.5f, 0.5f, 0);
        glTexCoord2f(0.0,1.0); //jobb felső
        glVertex3f( -0.5f, 0.5f, 0);
    glEnd();
}

float distance(float x1, float x2, float y1, float y2)
{
    float distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return distance;
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, 1280, 720, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
