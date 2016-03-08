#ifndef FACE_H
#define FACE_H

#include <la.h>
#include <scene/joint.h>
#include <QListWidgetItem>

class HalfEdge;

class Vertex : public QListWidgetItem
{
public:
    static int currId;

    Vertex(const glm::vec3 &pos);
    Vertex();

    glm::vec3 position;
    HalfEdge* edge;
    int id;

    glm::vec3 bindPosition;
    std::pair<Joint*,float> influences[2];
};

class Face : public QListWidgetItem
{
public:
    static int currId;

    Face(Vertex* idx1, Vertex* idx2, Vertex* idx3);
    Face();
    Vertex* vertices[3];

    HalfEdge* start_edge;
    glm::vec4 color;
    int id;
};

class HalfEdge : public QListWidgetItem
{
public:
    static int currId;

    HalfEdge(Face* face, Vertex* vert, HalfEdge* sym, HalfEdge* next);
    HalfEdge(Face* face, Vertex* vert);
    HalfEdge();

    Face* face;
    Vertex* vert;
    HalfEdge* sym;
    HalfEdge* next;
    int id;

};

#endif // FACE_H
