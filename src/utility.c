#include <math.h>
#include "utility.h"

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
