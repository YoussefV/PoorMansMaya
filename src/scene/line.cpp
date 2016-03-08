#include "line.h"

static const int LN_IDX_COUNT = 2;
static const int LN_VERT_COUNT = 2;
static const int LN_COL_COUNT = 2;

Line::Line() : Geometry(){
    this->p1 = glm::vec4(0,0,0,1);
    this->p2 = glm::vec4(0,0,0,1);
    this->color = glm::vec4(1,0,0,1);
    this->color2 = glm::vec4(0,1,0,1);
}

Line::Line(HalfEdge* edge) : Line() {
    this->v1 = edge->vert;
    this->v2 = edge->sym->vert;
    this->p1 = glm::vec4(v1->position,1);
    this->p2 = glm::vec4(v2->position,1);
}

void Line::create() {
    glm::vec4 ln_vert_pos[LN_VERT_COUNT];
    GLuint ln_idx[LN_IDX_COUNT];
    glm::vec4 ln_col[LN_COL_COUNT];

    if (v1->influences[0].second != 0 && v1->influences[1].second != 0) {

    glm::mat4 currTrans1 = v1->influences[0].first->getOverallTransformation();
    glm::mat4 currTrans2 = v1->influences[1].first->getOverallTransformation();

    glm::mat4 bindMat1 = *v1->influences[0].first->getBindMatrix();
    glm::mat4 bindMat2 = *v1->influences[1].first->getBindMatrix();

    float influence1 = v1->influences[0].second;
    float influence2 = v1->influences[1].second;

    p1 = (influence1 * currTrans1 * bindMat1 * glm::vec4(v1->bindPosition,1)) +
            (influence2 * currTrans2 * bindMat2 * glm::vec4(v1->bindPosition,1));

    currTrans1 = v2->influences[0].first->getOverallTransformation();
    currTrans2 = v2->influences[1].first->getOverallTransformation();

    bindMat1 = *v2->influences[0].first->getBindMatrix();
    bindMat2 = *v2->influences[1].first->getBindMatrix();

    influence1 = v2->influences[0].second;
    influence2 = v2->influences[1].second;

    p2 = (influence1 * currTrans1 * bindMat1 * glm::vec4(v2->bindPosition,1)) +
            (influence2 * currTrans2 * bindMat2 * glm::vec4(v2->bindPosition,1));
    }

    ln_vert_pos[0] = p1;
    ln_vert_pos[1] = p2;

    ln_idx[0] = 0;
    ln_idx[1] = 1;

    ln_col[0] = color;
    ln_col[1] = color2;


    count = LN_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(ln_idx, LN_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(ln_vert_pos,LN_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(ln_col, LN_COL_COUNT * sizeof(glm::vec4));

}

GLenum Line::drawMode() {
    return GL_LINES;
}

void Line::setColor(glm::vec4 color) {
    this->color = color;
    this->color2 = color;
}

void Line::setColor(glm::vec4 color1, glm::vec4 color2) {
    this->color = color1;
    this->color2 = color2;
}

bool Line::initialized() {
    return p1==p2;
}
