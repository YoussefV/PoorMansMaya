#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <scene/geometry.h>
#include <scene/face.h>

class Triangle : public Geometry
{
private:
    Face* f;
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
    glm::vec4 color2;
    glm::vec4 color3;

public:
    Triangle();
    Triangle(glm::vec4 x, glm::vec4 y, glm::vec4 z) :Triangle(){
        p1 = x;
        p2 = y;
        p3 = z;
    }

    Triangle(Face*);
    virtual void create();
    virtual GLenum drawMode();

    void setColor(glm::vec4);
    void setColor(glm::vec4,glm::vec4,glm::vec4);

};

#endif // TRIANGLE_H
