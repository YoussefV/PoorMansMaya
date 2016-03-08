#include "point.h"

Point::Point() : Geometry()
{
    this->color = glm::vec4(0,0,1,1);
}


Point::Point(Vertex* v) : Point() {
    this->p = glm::vec4(v->position,1);
    this->v = v;
}

void Point::create() {
    glm::vec4 pt_vert_pos[1];
    glm::vec4 pt_col[1];
    GLuint pt_idx[1];

    if ( v->influences[0].second != 0 &&  v->influences[1].second != 0) {
        glm::mat4 currTrans1 = v->influences[0].first->getOverallTransformation();
        glm::mat4 currTrans2 = v->influences[1].first->getOverallTransformation();

        glm::mat4 bindMat1 = *v->influences[0].first->getBindMatrix();
        glm::mat4 bindMat2 = *v->influences[1].first->getBindMatrix();

        float influence1 = v->influences[0].second;
        float influence2 = v->influences[1].second;

        p = (influence1 * currTrans1 * bindMat1 * glm::vec4(v->bindPosition,1)) +
                (influence2 * currTrans2 * bindMat2 * glm::vec4(v->bindPosition,1));
    }

    pt_vert_pos[0] = p;

    pt_idx[0] = 0;

    pt_col[0] = color;

    count = 1;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(0, sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pt_vert_pos, sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(pt_col, sizeof(glm::vec4));
}

GLenum Point::drawMode() {
    return GL_POINTS;
}
