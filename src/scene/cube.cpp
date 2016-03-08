#include "cube.h"
#include<la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

Cube::Cube() : Geometry()
{}

void createCubeVertexPositions(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT]){

    for (int i= 0; i<4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*90.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);
        cub_vert_pos[i] = v;
    }

    for (int i= 4; i<8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-4)*90.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0.5f,1);
        cub_vert_pos[i] = v;
    }

    for (int i= 0; i<4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*90.0f, glm::vec3(1, 0, 0)) * glm::vec4(0.5f,0.5f,0.5f,1);;
        cub_vert_pos[i+8] = v;
    }

    for (int i= 4; i<8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-4)*90.0f, glm::vec3(1, 0, 0)) * glm::vec4(-0.5f,0.5f,0.5f,1);
        cub_vert_pos[i+8] = v;
    }

    for (int i= 0; i<4; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*90.0f, glm::vec3(0, 0, 1)) * glm::vec4(0.5f,0.5f,0.5f,1);
        cub_vert_pos[i+16] = v;
    }

    for (int i= 4; i<8; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), (i-4)*90.0f, glm::vec3(0, 0, 1)) * glm::vec4(0.5f,0.5f,-0.5f,1);
        cub_vert_pos[i+16] = v;
    }

}




void createCubeVertexNormals(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT]){
    //Store top plane normals (IDX 0 - 4)
    for(int i = 0; i < 4; i++){
        cub_vert_nor[i] =  glm::vec4(0,1,0,0);
    }
    //Store bottom plane normals (IDX 4 - 7)
    for(int i = 4; i < 8; i++){
        cub_vert_nor[i] =  glm::vec4(0,-1,0,0);
    }

    //Store sides of cube normals (IDX 8 - 11)
    for(int i = 0; i < 4; i++){
        glm::vec4 v =  glm::vec4(1,0,0,0);
        cub_vert_nor[i + 8] = v;
    }
    //Store sides of cube normals (IDX 12 - 15)
    for(int i = 4; i < 8; i++){
        glm::vec4 v =  glm::vec4(-1,0,0,0);
        cub_vert_nor[i + 8] = v;
    }

    //Store front of cube normals (IDX 16 - 19)
    for(int i = 0; i < 4; i++){
        glm::vec4 v =  glm::vec4(0,0,1,0);
        cub_vert_nor[i + 16] = v;
    }
    //Store back of cube normals (IDX 20 - 23)
    for(int i = 4; i < 8; i++){
        glm::vec4 v =  glm::vec4(0,0,-1,0);
        cub_vert_nor[i + 16] = v;
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT]){
    cub_idx[0] = 0;
    cub_idx[1] = 1;
    cub_idx[2] = 2;

    cub_idx[3] = 0;
    cub_idx[4] = 2;
    cub_idx[5] = 3;

    cub_idx[6] = 4;
    cub_idx[7] = 5;
    cub_idx[8] = 6;

    cub_idx[9] = 4;
    cub_idx[10] = 6;
    cub_idx[11] = 7;

    cub_idx[12] = 12;
    cub_idx[13] = 13;
    cub_idx[14] = 14;

    cub_idx[15] = 12;
    cub_idx[16] = 14;
    cub_idx[17] = 15;

    cub_idx[18] = 8;
    cub_idx[19] = 9;
    cub_idx[20] = 10;

    cub_idx[21] = 8;
    cub_idx[22] = 10;
    cub_idx[23] = 11;

    cub_idx[24] = 16;
    cub_idx[25] = 17;
    cub_idx[26] = 18;

    cub_idx[27] = 16;
    cub_idx[28] = 18;
    cub_idx[29] = 19;

    cub_idx[30] = 20;
    cub_idx[31] = 21;
    cub_idx[32] = 22;

    cub_idx[33] = 20;
    cub_idx[34] = 22;
    cub_idx[35] = 23;
}

void Cube::create() {
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec4 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec4 cub_vert_nor[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    count = CUB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, CUB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos,CUB_VERT_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, CUB_VERT_COUNT * sizeof(glm::vec4));
}
