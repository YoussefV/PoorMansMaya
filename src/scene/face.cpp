#include "face.h"

int Face::currId = 0;
int Vertex::currId = 0;
int HalfEdge::currId = 0;

/**** Face Constructors ****/
Face::Face(Vertex* idx1, Vertex* idx2, Vertex* idx3)
    : vertices {idx1, idx2, idx3} {
    id = currId;
    setText(QString::number(id));
    currId++;
}

Face::Face() : Face(nullptr,nullptr,nullptr) {
    id = currId;
    setText(QString::number(id));
    currId++;
}


/**** Vertex Constructors ****/
Vertex::Vertex(const glm::vec3 &pos)
    : position(pos){
    id = currId;
    setText(QString::number(id));
    currId++;
}

Vertex::Vertex() : Vertex(glm::vec3(0)) {
    id = currId;
    setText(QString::number(id));
    currId++;
}



/**** Half Edge Constructors ****/
HalfEdge::HalfEdge(Face* face, Vertex* vert, HalfEdge* sym, HalfEdge* next) {
    this->face = face;
    this->vert = vert;
    this->sym = sym;
    this->next = next;
    id = currId;
    setText(QString::number(id));
    currId++;
}

HalfEdge::HalfEdge(Face* face, Vertex* vert) {
    this->face = face;
    this->vert = vert;
    id = currId;
    setText(QString::number(id));
    currId++;
}

HalfEdge::HalfEdge() {
    id = currId;
    setText(QString::number(id));
    currId++;
}
