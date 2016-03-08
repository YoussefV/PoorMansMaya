#ifndef LINE_H
#define LINE_H

#include <scene/geometry.h>
#include <scene/face.h>

class Line : public Geometry
{
private:
    Vertex* v1;
    Vertex* v2;
    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 color2;

public:
    Line();
    Line(HalfEdge*);
    virtual void create();
    virtual GLenum drawMode();
    bool initialized();

    void setColor(glm::vec4);
    void setColor(glm::vec4,glm::vec4);
};

#endif // LINE_H
