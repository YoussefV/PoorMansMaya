#pragma once
#include <scene/geometry.h>
#include <scene/joint.h>
#include <QVector>
#include <la.h>
#include <utility>
#include <vector>
#include <scene/face.h>
#include <meshshaderprogram.h>

class MeshShaderProgram;

class Mesh : public Geometry
{
public:
    Mesh();
    virtual ~Mesh(){}
    virtual void create();
    virtual void destroy();
    bool LoadOBJ(const QString &filepath, GLWidget277* mygl);
    void bindVertices(Joint*);
    void updateVertices(Joint*, MeshShaderProgram&);
    Joint* selection;
//    void selectionChanged(Joint*); //set unif selected joint to sth else

    virtual bool bindInfluences();
    virtual bool bindJointIDs();

    //Like slots, send data to mygl to be sent to respective widgets
    void sendVertices(GLWidget277* mygl);
    void sendFaces(GLWidget277* mygl);
//    void sendHalfEdges(GLWidget277* mygl,std::map<std::pair<int, int>, HalfEdge* >);


protected:
    QOpenGLBuffer bufInfluences;
    QOpenGLBuffer bufJointIDs;

private:
    QVector<Face*> faces;
    QVector<Vertex*> vertices;
    QVector<HalfEdge> halfedges;

    //When computing the normal of a Face for its VBOs, just take the cross product of two of its edges.
    //No need to store the normals unless you want smooth-looking surfaces
};
