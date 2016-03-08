#include "pipe.h"
#include <la.h>
#include <iostream>

static const int pipe_IDX_COUNT = 480;
static const int pipe_VERT_COUNT = 160;

Pipe::Pipe() : Geometry()
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createPipeVertexPositions(glm::vec4 (&pipe_vert_pos)[pipe_VERT_COUNT]){
    //Create two rings of vertices
    //We'll be using 20-sided polygons to approximate circles
    //for the endcaps


    //LARGE
    //Store top of barrel verts (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,1,0,1);
        pipe_vert_pos[i] = v;
    }
    //Store bottom of barrel verts (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,-1,0,1);
        pipe_vert_pos[i] = v;
    }
    //SMALL
    //Store top of barrel verts (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,1,0,1);
        pipe_vert_pos[i+40] = v;
    }
    //Store bottom of barrel verts (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-1,0,1);
        pipe_vert_pos[i+40] = v;
    }


    //FOR CAPS
    //LARGE
    //Store top of barrel verts (IDX 80 - 99)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,1,0,1);
        pipe_vert_pos[i+80] = v;
    }
    //Store bottom of barrel verts (IDX 100 - 119)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,-1,0,1);
        pipe_vert_pos[i+80] = v;
    }


    //SMALL
    //Store top of barrel verts (IDX 120 - 139)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,1,0,1);
        pipe_vert_pos[i+120] = v;
    }
    //Store bottom of barrel verts (IDX 140 - 159)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-1,0,1);
        pipe_vert_pos[i+120] = v;
    }


}


void createPipeVertexNormals(glm::vec4 (&pipe_vert_nor)[pipe_VERT_COUNT]){

    //Store top of barrel normals (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i] = v;
    }
    //Store bottom of barrel normals (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i] = v;
    }

    //Store top of barrel normals (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i+40] = v;
    }
    //Store bottom of barrel normals (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-20)*18.0f, glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i+40] = v;
    }


    //FOR CAPS
    //FOR LARGE
    //Store top cap normals (IDX 80 - 99)
    for(int i = 80; i < 100; i++){
        pipe_vert_nor[i] =  glm::vec4(0,1,0,0);
    }
    //Store bottom cap normals (IDX 100 - 119)
    for(int i = 100; i < 120; i++){
        pipe_vert_nor[i] =  glm::vec4(0,-1,0,0);
    }

    //FOR SMALL
    //Store top cap normals (IDX 120 - 139)
    for(int i = 120; i < 139; i++){
        pipe_vert_nor[i] =  glm::vec4(0,1,0,0);
    }
    //Store bottom cap normals (IDX 140 - 159)
    for(int i = 140; i < 160; i++){
        pipe_vert_nor[i] =  glm::vec4(0,-1,0,0);
    }
}


void createPipeIndices(GLuint (&pipe_idx)[pipe_IDX_COUNT]){

    //Build indices for the sides of the Pipe
    for(int i = 0; i < 19; i++){
        pipe_idx[i*6] = i;
        pipe_idx[1 + i*6] = i + 1;
        pipe_idx[2 + i*6] = i + 20;
        pipe_idx[3 + i*6] = i + 1;
        pipe_idx[4 + i*6] = i + 21;
        pipe_idx[5 + i*6] = i + 20;
    }
    //Fixing last triangles
    pipe_idx[114] = 19;
    pipe_idx[115] = 20;
    pipe_idx[116] = 39;

    pipe_idx[117] = 0;
    pipe_idx[118] = 19;
    pipe_idx[119] = 20;

    //Build indices for the sides of the Pipe
    for(int i = 20; i < 39; i++){
        pipe_idx[i*6] = i + 20;
        pipe_idx[1 + i*6] = i + 21;
        pipe_idx[2 + i*6] = i + 40;
        pipe_idx[3 + i*6] = i + 21;
        pipe_idx[4 + i*6] = i + 41;
        pipe_idx[5 + i*6] = i + 40;
    }
    //Fixing last triangles
    pipe_idx[234] = 59;
    pipe_idx[235] = 60;
    pipe_idx[236] = 79;

    pipe_idx[237] = 40;
    pipe_idx[238] = 59;
    pipe_idx[239] = 60;

    //Build indices for the caps of the Pipe
    for(int i = 40; i < 59; i++){
        pipe_idx[i*6] = i + 80;
        pipe_idx[1 + i*6] = i + 81;
        pipe_idx[2 + i*6] = i + 40;
        pipe_idx[3 + i*6] = i + 81;
        pipe_idx[4 + i*6] = i + 40;
        pipe_idx[5 + i*6] = i + 41;
    }

    pipe_idx[354] = 120;
    pipe_idx[355] = 99;
    pipe_idx[356] = 80;

    pipe_idx[357] = 139;
    pipe_idx[358] = 120;
    pipe_idx[359] = 99;

    //Build indices for the caps of the Pipe
    for(int i = 60; i < 79; i++){
        pipe_idx[i*6] = i + 80;
        pipe_idx[1 + i*6] = i + 81;
        pipe_idx[2 + i*6] = i + 40;
        pipe_idx[3 + i*6] = i + 81;
        pipe_idx[4 + i*6] = i + 40;
        pipe_idx[5 + i*6] = i + 41;
    }

    pipe_idx[474] = 140;
    pipe_idx[475] = 119;
    pipe_idx[476] = 100;

    pipe_idx[477] = 159;
    pipe_idx[478] = 140;
    pipe_idx[479] = 119;

}

void Pipe::create()
{
    GLuint pipe_idx[pipe_IDX_COUNT];
    glm::vec4 pipe_vert_pos[pipe_VERT_COUNT];
    glm::vec4 pipe_vert_nor[pipe_VERT_COUNT];

    createPipeVertexPositions(pipe_vert_pos);
    createPipeVertexNormals(pipe_vert_nor);
    createPipeIndices(pipe_idx);

    count = pipe_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(pipe_idx, pipe_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pipe_vert_pos,pipe_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(pipe_vert_nor, pipe_VERT_COUNT * sizeof(glm::vec4));
}
