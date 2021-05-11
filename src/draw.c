#include "draw.h"

#include <GL/glut.h>

void draw_model(const Model* model)
{
    glPushMatrix();
        glTranslatef(model->position.x, model->position.y, model->position.z); //modell mozgat�sa
        glRotatef(model->rotation.z, 0, 0, 1); //modell forgat�sa
        glRotatef(model->rotation.y, 0, 1, 0);
        glRotatef(model->rotation.x, 1, 0, 0);
        glBindTexture(GL_TEXTURE_2D, model->texture); //modell text�r�j�nak bindol�sa
        draw_triangles(model); //h�romsz�gek kirajzol�sa
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
    //text�ra bindol�s
    glBindTexture(GL_TEXTURE_2D, texture);

    //poz�cion�l�s, forgat�s, �tm�retez�s
    glTranslatef(pos_x, pos_y, pos_z);
    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);
    glRotatef(rotate_z, 0, 0, 1);
    glScalef(scale, scale, scale);

    //kirajzol�s
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glTexCoord2f(0.0,0.0); //bal fels� sarok
        glVertex3f( -0.5f, -0.5f, 0);
        glTexCoord2f(1.0,0.0); //bal als� sarok
        glVertex3f( 0.5f, -0.5f, 0);
        glTexCoord2f(1.0,1.0); //jobb als�
        glVertex3f( 0.5f, 0.5f, 0);
        glTexCoord2f(0.0,1.0); //jobb fels�
        glVertex3f( -0.5f, 0.5f, 0);
    glEnd();
    glPopMatrix();
}
