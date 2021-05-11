#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include "load.h"
#include "transform.h"
#include "draw.h"
#include "texture.h"
#include "utility.h"

//=========================================================================================================================================
//kamera
Camera camera; //stuktúra + változónév
//kurzor
Cursor cursor;
//fény változók
GLfloat lightPosition[] = {1, 1, 1, 0.0};
GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};
GLfloat globalAmbientLight[] = {0.1, 0.1, 0.1, 1.0};
bool lightOn = true;
//modellek
Model meat;
Model pan;
Model stove;
Model refrigerator;
Model table;
Model lightSwitch;
//textúrák
GLuint tex_floor;
GLuint tex_wall;
GLuint tex_info;
GLuint tex_glow;
//hús pozíciója
float meat_speed = 0.05;
//sütő állapot, tepsi izzás
bool stoveOn = false;
float glow_level = 0;
//info ablak változó
bool showInfo = false;

//=========================================================================================================================================
void init(int argc, char* argv[]);
void loadModels();
void loadTextures();
void renderScene();
void reshape();
void mouseMotion();
void mouse();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void idle();

//=========================================================================================================================================
int main(int argc, char* argv[])
{
    //openGL inicializálás
    init(argc, argv);

    //modellek betöltése
    loadModels();
    //textúrák betöltése
    loadTextures();

	//render
	glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouseMotion);
    //glutMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}

//=========================================================================================================================================
void init(int argc, char* argv[])
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

	glClearColor(0.8, 0.8, 0.8, 0.0);
    glClearDepth(1.0);
	glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //kamera beállítása
    camera.position.z = 10;
    camera.rotation.z = 90;
    camera.distance = 10;
    camera.speed.z = 0.01;

    //fény beállítása
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);
}

void loadModels()
{
    //hús
    load_model(&meat, "models/raw_meat.obj");
    scale_model(&meat, 0.5, 0.5, 0.5);
    //serpenyő
    load_model(&pan, "models/frying_pan.obj");
    //sütő
    load_model(&stove, "models/stove.obj");
    scale_model(&stove, 0.2, 0.2, 0.2);
    set_model_position(&stove, 5, -1, 0);
    stove.rotation.z = 90;
    //hűtő
    load_model(&refrigerator, "models/refrigerator.obj");
    scale_model(&refrigerator, 0.4, 0.4, 0.4);
    set_model_position(&refrigerator, -30, 0, -18);
    refrigerator.rotation.z = -90;
    //asztal
    load_model(&table, "models/table.obj");
    scale_model(&table, 0.2, 0.2, 0.2);
    set_model_position(&table, -40, 50, -18);
    //villanykapcsoló
    load_model(&lightSwitch, "models/light_switch.obj");
    scale_model(&lightSwitch, 0.25, 0.25, 0.25);
    set_model_position(&lightSwitch, 30, -9, 5);
    lightSwitch.rotation.z = 90;
}

void loadTextures()
{
    meat.texture = load_texture("textures/raw_meat.png");
    pan.texture = load_texture("textures/frying_pan.jpg");
    stove.texture = load_texture("textures/stove.png");
    refrigerator.texture = load_texture("textures/refrigerator.jpg");
    table.texture = load_texture("textures/table.png");
    lightSwitch.texture = load_texture("textures/light_switch.png");
    tex_floor = load_texture("textures/tile_floor.jpg");
    tex_wall = load_texture("textures/wall.png");
    tex_info = load_texture("textures/info.png");
    tex_glow = load_texture("textures/red_glow.png");
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
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

        //hús kirajzoláas
        draw_model(&meat);
        //serpenyő kirajzolása
        draw_model(&pan);
        //sütő kirajzolása
        draw_model(&stove);
        //hűtő kirajzolása
        draw_model(&refrigerator);
        //asztal kirajzolása
        draw_model(&table);

        //padló kirajzolása
        drawTexturedRectangle(tex_floor, 0, 30, -18, 0, 0, 0, 100);

        //fal kirajzolása
        drawTexturedRectangle(tex_wall, 0, -9, 2, -90, 0, 0, 100);
        drawTexturedRectangle(tex_wall, 50, 30, 2, 0, 90, 0, 100);
        drawTexturedRectangle(tex_wall, -50, 30, 2, 0, 90, 0,100);

        //villanykapcsoló kirajzolása
        draw_model(&lightSwitch);

        //izzás kirajzolása
        glPushMatrix();
            glColor4f(1.0, 1.0, 1.0, glow_level);//átlátszóság beállítása
            glDisable(GL_LIGHTING); //árnyalás kikapcsolása, hogy ragyogjon
            drawTexturedRectangle(tex_glow, 0, 0, 0.1, 0, 0, 0, 5);
            glEnable(GL_LIGHTING); //árnyalás visszakapcsolása
            glColor4f(1.0f,1.0f,1.0f, 1.0f); //átltászóság visszaállítása
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
        if(distance(meat.position.x - meat_speed, 0, meat.position.y, 0) < 1.5)
        {
            meat.position.x -= meat_speed;
        }
        break;
    case 'a':
        if(distance(meat.position.x + meat_speed, 0, meat.position.y, 0) < 1.5)
        {
            meat.position.x += meat_speed;
        }
        break;
    case 'w':
        if(distance(meat.position.x, 0, meat.position.y - meat_speed, 0) < 1.5)
        {
            meat.position.y -= meat_speed;
        }
        break;
    case 's':
        if(distance(meat.position.x, 0, meat.position.y + meat_speed, 0) < 1.5)
        {
            meat.position.y += meat_speed;
        }
        break;
    case 'l':
        if(lightOn)
        {
            lightSwitch.rotation.x = 180;

            glDisable(GL_LIGHT0);
            lightOn = false;
        }
        else if(!lightOn)
        {
            lightSwitch.rotation.x = 0;

            glEnable(GL_LIGHT0);
            lightOn = true;
        }
        break;
    case 'o':
        if(stoveOn)
        {
            stoveOn = false;
        }
        else if(!stoveOn)
        {
            stoveOn = true;
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

void idle()
{
    if(stoveOn && glow_level < 1)
    {
        glow_level += 0.001;
    }
    else if(!stoveOn && glow_level > 0)
    {
        glow_level -= 0.001;
    }
}
