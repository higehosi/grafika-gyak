#include "draw.h"

#include <GL/glut.h>

void draw_model(const Model* model)
{
    glPushMatrix();
        glTranslatef(model->position.x, model->position.y, model->position.z); //modell mozgatása
        glRotatef(model->rotation.z, 0, 0, 1); //modell forgatása
        glRotatef(model->rotation.y, 0, 1, 0);
        glRotatef(model->rotation.x, 1, 0, 0);
        glBindTexture(GL_TEXTURE_2D, model->texture); //modell textúrájának bindolása
        draw_triangles(model); //háromszögek kirajzolása
    glPopMatrix();
}

void draw_triangles(const Model* model)
{
    int i, k;
    int vertex_index, texture_index, normal_index;
    float x, y, z, u, v;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {

            normal_index = model->triangles[i].points[k].normal_index;
            x = model->normals[normal_index].x;
            y = model->normals[normal_index].y;
            z = model->normals[normal_index].z;
            glNormal3f(x, y, z);

            texture_index = model->triangles[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1.0 - v);

            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}

void drawTexturedRectangle(GLuint texture, float pos_x, float pos_y, float pos_z, float rotate_x, float rotate_y, float rotate_z, float scale)
{
    glPushMatrix();
    //textúra bindolás
    glBindTexture(GL_TEXTURE_2D, texture);

    //pozícionálás, forgatás, átméretezés
    glTranslatef(pos_x, pos_y, pos_z);
    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);
    glRotatef(rotate_z, 0, 0, 1);
    glScalef(scale, scale, scale);

    //kirajzolás
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0.0,0.0); //bal felsõ sarok
        glVertex3f( -0.5f, -0.5f, 0);
        glTexCoord2f(1.0,0.0); //bal alsó sarok
        glVertex3f( 0.5f, -0.5f, 0);
        glTexCoord2f(1.0,1.0); //jobb alsó
        glVertex3f( 0.5f, 0.5f, 0);
        glTexCoord2f(0.0,1.0); //jobb felsõ
        glVertex3f( -0.5f, 0.5f, 0);
    glEnd();
    glPopMatrix();
}
