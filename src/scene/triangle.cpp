#include "triangle.h"

static const int TR_IDX_COUNT = 3;
static const int TR_VERT_COUNT = 3;
static const int TR_COL_COUNT = 3;

Triangle::Triangle() : Geometry() {
    this->p1 = glm::vec4(0,0,0,1);
    this->p2 = glm::vec4(0,0,0,1);
    this->p3 = glm::vec4(0,0,0,1);
    this->color =  glm::vec4(1,1,1,1);
    this->color2 = glm::vec4(1,1,1,1);
    this->color3 = glm::vec4(1,1,1,1);
}

Triangle::Triangle(Face * f) : Triangle() {
    this->f = f;
    this->p1 = glm::vec4(f->vertices[0]->position,1);
    this->p2 = glm::vec4(f->vertices[1]->position,1);
    this->p3 = glm::vec4(f->vertices[2]->position,1);

    //To Avoid Z-Fighting:
    glm::vec4 pushOut =  0.02f * glm::vec4(glm::cross(glm::vec3(p1-p3),glm::vec3(p1-p2)),1);
    pushOut *= 0.1f; //Because of floater inprecision, I cant just multiply by 0.002f above
    p1-=pushOut;
    p2-=pushOut;
    p3-=pushOut;
}

void Triangle::create() {
    glm::vec4 tr_vert_pos[TR_VERT_COUNT];
    glm::vec4 tr_nor[TR_VERT_COUNT];
    GLuint    tr_idx[TR_IDX_COUNT];
    glm::vec4 tr_col[TR_COL_COUNT];

    if (f->vertices[0]->influences[0].second != 0 && f->vertices[0]->influences[1].second != 0) {

    glm::mat4 currTrans1 = f->vertices[0]->influences[0].first->getOverallTransformation();
    glm::mat4 currTrans2 = f->vertices[0]->influences[1].first->getOverallTransformation();

    glm::mat4 bindMat1 = *f->vertices[0]->influences[0].first->getBindMatrix();
    glm::mat4 bindMat2 = *f->vertices[0]->influences[1].first->getBindMatrix();

    float influence1 = f->vertices[0]->influences[0].second;
    float influence2 = f->vertices[0]->influences[1].second;

    p1 = (influence1 * currTrans1 * bindMat1 * glm::vec4(f->vertices[0]->bindPosition,1)) +
            (influence2 * currTrans2 * bindMat2 * glm::vec4(f->vertices[0]->bindPosition,1));

    currTrans1 = f->vertices[1]->influences[0].first->getOverallTransformation();
    currTrans2 = f->vertices[1]->influences[1].first->getOverallTransformation();

    bindMat1 = *f->vertices[1]->influences[0].first->getBindMatrix();
    bindMat2 = *f->vertices[1]->influences[1].first->getBindMatrix();

    influence1 = f->vertices[1]->influences[0].second;
    influence2 = f->vertices[1]->influences[1].second;

    p2 = (influence1 * currTrans1 * bindMat1 * glm::vec4(f->vertices[1]->bindPosition,1)) +
            (influence2 * currTrans2 * bindMat2 * glm::vec4(f->vertices[1]->bindPosition,1));

    currTrans1 = f->vertices[2]->influences[0].first->getOverallTransformation();
    currTrans2 = f->vertices[2]->influences[1].first->getOverallTransformation();

    bindMat1 = *f->vertices[2]->influences[0].first->getBindMatrix();
    bindMat2 = *f->vertices[2]->influences[1].first->getBindMatrix();

    influence1 = f->vertices[2]->influences[0].second;
    influence2 = f->vertices[2]->influences[1].second;

    p3 = (influence1 * currTrans1 * bindMat1 * glm::vec4(f->vertices[2]->bindPosition,1)) +
            (influence2 * currTrans2 * bindMat2 * glm::vec4(f->vertices[2]->bindPosition,1));

    }


    tr_vert_pos[0] = p1;
    tr_vert_pos[1] = p2;
    tr_vert_pos[2] = p3;

    tr_idx[0] = 0;
    tr_idx[1] = 1;
    tr_idx[2] = 2;

    tr_col[0] = color;
    tr_col[1] = color2;
    tr_col[2] = color3;

    tr_nor[0] = glm::vec4(glm::cross(glm::vec3(p2-p1),glm::vec3(p3-p1)),0);
    tr_nor[1] = glm::vec4(glm::cross(glm::vec3(p3-p2),glm::vec3(p1-p2)),0);
    tr_nor[2] = glm::vec4(glm::cross(glm::vec3(p1-p3),glm::vec3(p2-p3)),0);

    count = TR_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(tr_idx, TR_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(tr_vert_pos,TR_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(tr_col, TR_COL_COUNT * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(tr_nor, TR_VERT_COUNT * sizeof(glm::vec4));
}

void Triangle::setColor(glm::vec4 color) {
    this->color = color;
    this->color2 = color;
    this->color3 = color;
}

void Triangle::setColor(glm::vec4 color1, glm::vec4 color2, glm::vec4 color3) {
    this->color = color1;
    this->color2 = color2;
    this->color3 = color3;
}

GLenum Triangle::drawMode() {
    return GL_TRIANGLES;
}
