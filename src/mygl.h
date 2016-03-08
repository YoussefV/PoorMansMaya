#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/pipe.h>
#include <scene/character.h>
#include <scene/cone.h>
#include <scene/cube.h>
#include <scene/sphere.h>
#include <scene/node.h>
#include <scene/line.h>
#include <vector>
#include <scene/camera.h>
#include <scene/joint.h>
#include <scene/mesh.h>
#include <meshshaderprogram.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <iostream>


class MyGL
    : public GLWidget277
{

private:
    QOpenGLVertexArrayObject vao;

    Sphere geom_sphere;//The instance of a unit cylinder we can use to render any cylinder
    Cube geom_cube;//The instance of a unit cube we can use to render any sphere
    Cylinder geom_cylinder;
    Cone geom_cone;
    Pipe geom_pipe;

    HalfEdge* edgeSelection;
    Face* faceSelection;
    Vertex* vertexSelection;

    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;
    MeshShaderProgram prog_mesh;
    MeshShaderProgram prog_selection;

public:
    Camera camera;
    std::vector<Character*> items;
    std::vector<Joint*> joints;
    std::vector<Mesh*> meshes;
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    virtual void slot_skeletonButtonPressed();
    virtual void slot_meshButtonPressed();
    virtual void slot_bindAtIndex(int);
    virtual void slot_updateVertices();

    virtual void slot_halfEdgeSelectionChanged(QListWidgetItem*);
    virtual void slot_nextEdgeButtonPressed();
    virtual void slot_symEdgeButtonPressed();

    virtual void slot_faceSelectionChanged(QListWidgetItem *);

    virtual void slot_vertexSelectionChanged(QListWidgetItem *);
    virtual void slot_receiveVertexTranslateX(double);
    virtual void slot_receiveVertexTranslateY(double);
    virtual void slot_receiveVertexTranslateZ(double);

    virtual void slot_selectionID(int);


protected:
    bool event(QEvent * e);
    QPoint mouseInitPosition = QPoint(10000,10000);

};


#endif // MYGL_H
