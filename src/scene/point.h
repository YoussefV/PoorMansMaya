#ifndef POINT_H
#define POINT_H

#include <scene/geometry.h>
#include <scene/face.h>

class Point : public Geometry
{
private:
    glm::vec4 p;
    Vertex* v;

protected:
    QOpenGLBuffer bufInfluences;
    QOpenGLBuffer bufJointIDs;

public:
    Point();
    // Point(vertselectio*) {
    // on create, copy all data, inlcuding weights and ids
    //}
    Point(Vertex* v);
    virtual void create();
    virtual GLenum drawMode();
};

#endif // POINT_H
