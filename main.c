#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

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
} Camera;

//kamera
Camera camera;
//fény változók
GLfloat lightPosition[] = {1, 1, 1, 0.};
GLfloat lightColor[] = {0.7, 0.6, 0.6, 1.0};
GLfloat globalAmbientLight[] = {0.3, 0.3, 0.3, 1.0};

void renderScene();
void reshape();

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

    //kamera beállítása
    camera.position.x = 1;
    camera.position.y = 1;
    camera.position.z = 1;

    //fény beállítása
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

	//render
	glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}

void renderScene()
{
    //buffer ürítés
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//render
	glPushMatrix();
        //kamera pozícionálása
        gluLookAt(
                  camera.position.x, camera.position.y, camera.position.z,  //nézõpont
                  0, 0, 0,                                                  //nézett pont
                  0.0, 0.0, 1.0                                             //fel iránya
                  );


	glPopMatrix();

	//képcsere
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height)
{
	glViewport (0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.01, 500.0);
}
