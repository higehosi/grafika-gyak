#ifndef OBJ_DRAW_H
#define OBJ_DRAW_H

#include "model.h"

/**
 * Draw the model.
 */
void draw_model(const Model* model);

/**
 * Draw the triangles of the model.
 */
void draw_triangles(const Model* model);

/*
n�gyzet kirajzol�sa text�r�val
*/
void drawTexturedRectangle(GLuint texture, float pos_x, float pos_y, float pos_z, float rotate_x, float rotate_y, float rotate_z, float scale);

#endif /* OBJ_DRAW_H */
