#include "transform.h"

void scale_model(Model* model, double sx, double sy, double sz)
{
    int i;

    for (i = 0; i < model->n_vertices; ++i) {
        model->vertices[i].x *= sx;
        model->vertices[i].y *= sy;
        model->vertices[i].z *= sz;
    }
}

void set_model_position(Model* model, float x, float y, float z)
{
    model->position.x = x;
    model->position.y = y;
    model->position.z = z;
}
