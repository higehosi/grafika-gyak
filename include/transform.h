#ifndef OBJ_TRANSFORM_H
#define OBJ_TRANSFORM_H

#include "model.h"

/**
 * Scale the loaded model.
 */
void scale_model(Model* model, double sx, double sy, double sz);
void set_model_position(Model* model, float x, float y, float z);

#endif /* OBJ_TRANSFORM_H */
