#include "cone.h"
#include <la.h>
#include <iostream>

static const int con_IDX_COUNT = 120;
static const int con_VERT_COUNT = 41;

Cone::Cone() : Geometry()
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createConeVertexPositions(glm::vec4 (&con_vert_pos)[con_VERT_COUNT]){
    //Create two rings of vertices
    //We'll be using 20-sided polygons to approximate circles
    //for the endcaps

    //Store bottom cap verts (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        con_vert_pos[i] = v;
    }

    //Create two more rings of vertices, this time for the barrel faces
    //We could use the same vertex rings as above, but GL only supports one normal per vertex
    //so we have to have two vertices per position in order to have sharp edges between the endcaps
    //and the barrel

    //Store bottom cap verts (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        con_vert_pos[i] = v;
    }

    con_vert_pos[40] = glm::vec4(0,0.5f,0,1);
}


void createConeVertexNormals(glm::vec4 (&con_vert_nor)[con_VERT_COUNT]){

    //Store side normals (IDX 20 - 39)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,1,0,0);
        con_vert_nor[i] = v;
    }

    //Store bottom cap normals (IDX 20 - 39)
    for(int i = 0; i < 20; i++){
        con_vert_nor[i+20] =  glm::vec4(0,-1,0,0);
    }
}


void createConeIndices(GLuint (&con_idx)[con_IDX_COUNT]){
    //Build indices for the top cap (18 tris, indices 0 - 53, up to vertex 19)
    for(int i = 0; i < 20; i++){
        con_idx[(i*3)] = i;
        con_idx[(i*3)+1] = i+1;
        con_idx[(i*3)+2] = 40;
    }

    for(int i = 0; i < 20; i++){
        con_idx[(i+20) * 3] = 0;
        con_idx[(i+20) * 3 + 1] = i+1;
        con_idx[(i+20) * 3 + 2] = i+2;
    }
}

void Cone::create()
{
    GLuint con_idx[con_IDX_COUNT];
    glm::vec4 con_vert_pos[con_VERT_COUNT];
    glm::vec4 con_vert_nor[con_VERT_COUNT];

    createConeVertexPositions(con_vert_pos);
    createConeVertexNormals(con_vert_nor);
    createConeIndices(con_idx);

    count = con_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(con_idx, con_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(con_vert_pos,con_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(con_vert_nor, con_VERT_COUNT * sizeof(glm::vec4));
}

