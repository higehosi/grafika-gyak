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
    float distance;
} Camera;

//kurzor struktúra
typedef struct Cursor
{
    vec3 position;
    vec3 movement;
}Cursor;

float distance(float x1, float x2, float y1, float y2);
void setOrthographicProjection();
void restorePerspectiveProjection();
