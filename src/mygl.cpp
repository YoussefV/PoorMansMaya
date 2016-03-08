#include"mygl.h"
#include <la.h>

#include <vector>
#include <iostream>
#include <scene/camera.h>
#include <QApplication>
#include <QKeyEvent>
#include <QPinchGesture>
#include <mainwindow.h>
#include <QFileDialog>
#include <scene/line.h>
#include <scene/triangle.h>
#include <scene/point.h>
#include <QErrorMessage>

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      camera(),
      vertexSelection(nullptr),
      edgeSelection(nullptr),
      faceSelection(nullptr)
{ }

MyGL::~MyGL()
{
    makeCurrent();

    vao.destroy();
    geom_cube.destroy();
    geom_cone.destroy();
    geom_cylinder.destroy();
    geom_pipe.destroy();
    geom_sphere.destroy();
    delete edgeSelection;
    delete vertexSelection;
    delete faceSelection;
    for (int i =0 ; i<meshes.size(); i++) {
        meshes[i]->destroy();
    }
}

void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    //Create the example sphere (you should delete this when you add your own code elsewhere)
    geom_sphere.setColor(glm::vec4(0,1,0,1));
    geom_sphere.create(); //Create Sphere
    geom_cube.setColor(glm::vec4(0,1,1,1));
    geom_cube.create(); //Create Cube
    geom_cone.create(); //Create Cone
    geom_pipe.create(); //Create Pipe
    geom_cylinder.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl",":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl",":/glsl/wire.frag.glsl");
    //Create and set up the mesh shader
    prog_mesh.create(":/glsl/mesh.vert.glsl",":/glsl/mesh.frag.glsl");
    //Create and setup the selection shader
    prog_selection.create(":/glsl/mesh.vert.glsl",":/glsl/wire.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();
}

//Overloaded version of inherited method that I added
void MyGL::resizeGL()
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    // vvv Like this. ---YKV---
    //Create Camera
    glm::mat4 viewproj = camera.createViewProjMatrix();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)
    QMatrix4x4 qviewproj = la::to_qmat(viewproj);

    prog_lambert.prog.bind();
    prog_lambert.prog.setUniformValue(prog_lambert.unifViewProj, qviewproj);

    prog_wire.prog.bind();
    prog_wire.prog.setUniformValue(prog_wire.unifViewProj, qviewproj);

    prog_mesh.prog.bind();
    prog_mesh.prog.setUniformValue(prog_mesh.unifViewProj, qviewproj);

    prog_selection.prog.bind();
    prog_selection.prog.setUniformValue(prog_selection.unifViewProj, qviewproj);

    printGLErrorLog();
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.

    // vvv Like this. ---YKV---
    //Create Camera
    glm::mat4 viewproj = camera.createViewProjMatrix();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)
    QMatrix4x4 qviewproj = la::to_qmat(viewproj);

    prog_lambert.prog.bind();
    prog_lambert.prog.setUniformValue(prog_lambert.unifViewProj, qviewproj);

    prog_wire.prog.bind();
    prog_wire.prog.setUniformValue(prog_wire.unifViewProj, qviewproj);

    prog_mesh.prog.bind();
    prog_mesh.prog.setUniformValue(prog_mesh.unifViewProj, qviewproj);

    prog_selection.prog.bind();
    prog_selection.prog.setUniformValue(prog_selection.unifViewProj, qviewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1,1,1));
    //Send the geometry's transformation matrix to the shader
    prog_lambert.setModelMatrix(model);
    prog_mesh.setModelMatrix(model);

    for (int i=0; i< meshes.size(); i++) {
        prog_mesh.draw(*this,*meshes[i]);
    }

    this->glDisable(GL_DEPTH_TEST);
    //Draw Characters
    for (int i = 0; i<joints.size();i++) {
        joints[i]->draw(*this,prog_wire);
    }
    this->glEnable(GL_DEPTH_TEST);

    //Draw Selections
    prog_wire.setModelMatrix(glm::mat4(1.0f));
    //Draw HalfEdge Selection
    if (edgeSelection != nullptr) {
        Line l = Line(edgeSelection);
        l.create();
        prog_wire.draw(*this,l);
    }
    //Draw Face Selection
    if (faceSelection != nullptr) {
        Triangle t = Triangle(faceSelection);
        t.create();
        prog_wire.draw(*this,t); //no shading wanted
    }
    //Draw Vertex Selection
    if (vertexSelection != nullptr) {
        //Point(vertextselection)
        Point p = Point(vertexSelection);
        p.create();
        // draw with deformation shader.
        // probably have two shaders, one that's
        // prog.create(deformation, lmabert)
        // prog.create(deformation, wire)
        prog_wire.draw(*this,p);
    }
}

bool MyGL::event(QEvent *e) {
    //Call parent method
    QWidget::event(e);
    //Do Not Delete This^

    //Zoom
    if (e->type() == QEvent::NativeGesture) {
       QNativeGestureEvent* native = static_cast<QNativeGestureEvent*>(e);
        if (native->gestureType() == Qt::ZoomNativeGesture) {
              double amount = native->value()*100;
              double total = camera.getZoom() - amount;
              camera.setZoom(total >= 0.5 && total <= 150 ? camera.getZoom()-amount : camera.getZoom());

              resizeGL();
              paintGL();
        }
    }
    //Rotate & Pan
    if (e->type() == QEvent::MouseMove) {
        QMouseEvent* mouse = static_cast<QMouseEvent*>(e);
        if(mouse->buttons() == Qt::LeftButton) {
            //If Shift is held Down
            if (mouse->modifiers() == Qt::ShiftModifier) {
                //If mouseInitPosition is unitialized (10000), set the current mouse position as it's initial
                if (mouseInitPosition.x() == 10000) {
                    mouseInitPosition = mouse->pos();
                }

                //Change in X or Y is the mouse's new - old positions
                float delX = (mouse->pos().x()-mouseInitPosition.x());
                delX /= 2.0; //<---- For aesthetics
                float delY = (mouse->pos().y()-mouseInitPosition.y());
                delY /= 2.0; //<---- For aesthetics

                //The new Position is a result of moving the camera along it's local up axis delY units.
                //To reach this we normalize the up vector, multiply it by delY, then add that to camera.
                glm::vec3 newPos = camera.getPosition() - (glm::normalize(camera.getUp()) * delY);
                newPos += (glm::normalize(camera.getLeft()) * delX);

                //Same movement for target
                glm::vec3 newTarget = camera.getTarget() - (glm::normalize(camera.getUp()) * delY);
                newTarget += (glm::normalize(camera.getLeft()) * delX);

                camera.setPosition(newPos);
                camera.setTarget(newTarget);

                //This is so the next time the mouse move event happens,
                //we calculate the change in mouse positions using this
                //as the initial position.
                mouseInitPosition = mouse->pos();//Reset Mouse Increment

            } else { //If no shift, then rotate
                if (mouseInitPosition.x() == 10000) {
                    mouseInitPosition = mouse->pos();
                }

                //To avoid weird flipping bug past Phi = 180, we limit Phi
                float newPhi = camera.getPhi() - (mouse->pos().y()-mouseInitPosition.y());
                if (newPhi < 180 && newPhi > 0) {
                    camera.setPhi(newPhi);
                }

                //Just in case user rotates in circles a lot of time
                //we make theta loop from 0-360.
                float newTheta = camera.getTheta() + (mouse->pos().x()-mouseInitPosition.x());
                if (newTheta > 360) {
                    camera.setTheta(newTheta - 360);
                } else {
                    camera.setTheta(newTheta);
                }
                mouseInitPosition = mouse->pos();
            }

            resizeGL();
            paintGL();
        }
    } else if (e->type() == QEvent::MouseButtonRelease) {
        //When user releases the mouse, 'uninitialze the mouseInitPosition'
        mouseInitPosition = QPoint(10000,10000);
    }

    return e->isAccepted();
}

void MyGL::slot_skeletonButtonPressed() {
    QString filepath = QFileDialog::getOpenFileName(0, QString("Load OBJ"), QString(), QString("*.json"));
    if(filepath.length() == 0) { return; }

    Joint* joint = new Joint();
    bool successful = joint->loadJSON(filepath);

    if(!successful) {
        delete joint;
    } else {
        joints.push_back(joint);
        emit sig_sendSceneItem(joint);
    }

}

void MyGL::slot_meshButtonPressed() {
    Mesh* mesh = new Mesh();
    QString filepath = QFileDialog::getOpenFileName(0, QString("Load OBJ"), QString(), QString("*.obj"));
    if(filepath.length() == 0)
    {
        return;
    }

    bool successful = mesh->LoadOBJ(filepath,this);
    if(!successful)
    {
        delete mesh;
        mesh = nullptr;
    } else {
        meshes.push_back(mesh);
    }
}

void MyGL::slot_bindAtIndex(int idx) {
    if (meshes.size() > idx && joints[idx]!=nullptr) {
        meshes[idx]->bindVertices(joints[idx]);

        glm::mat4 transMats[100];
        glm::mat4 bindMats[100];

        joints[0]->getTransMats(transMats);
        joints[0]->getBindMats(bindMats);

        prog_mesh.setBindMatrices(bindMats);
        prog_mesh.setTransMatrices(transMats);
    } else {
         QErrorMessage* errorMessageDialog = new QErrorMessage(this);
         errorMessageDialog->setWindowTitle(QString("Error: Root Joint Not Selected!"));
         errorMessageDialog->showMessage(QString("Select Root Joint to Bind"));
    }

}

void MyGL::slot_updateVertices() {
    //If something actually changed, and not just the selection:
    if (meshes.size() > 0) {
        meshes[0]->updateVertices(joints[0], prog_mesh); //Assuming that there's one mesh
    }
}

//Edge Code:
void MyGL::slot_halfEdgeSelectionChanged(QListWidgetItem * l) {
    if (HalfEdge* edge = dynamic_cast<HalfEdge *>(l)) {
        this->edgeSelection = edge;
    }
}

void MyGL::slot_nextEdgeButtonPressed() {
    if (edgeSelection != nullptr) {
        if (edgeSelection->next != nullptr) {
            edgeSelection = edgeSelection->next; //The selection in the widget doesn't update 504
        } else {
            QErrorMessage* errorMessageDialog = new QErrorMessage(this);
            errorMessageDialog->setWindowTitle(QString("Error: No next found!"));
            errorMessageDialog->showMessage(QString("This Half-Edge has no next edge value"));
        }
    }
}

void MyGL::slot_symEdgeButtonPressed() {
    if (edgeSelection != nullptr) {
        if (edgeSelection->sym != nullptr) {
            edgeSelection = edgeSelection->sym;
        } else {
            QErrorMessage* errorMessageDialog = new QErrorMessage(this);
            errorMessageDialog->setWindowTitle(QString("Error: No sym found!"));
            errorMessageDialog->showMessage(QString("This Half-Edge has no symmetrical edge value"));
        }
    }
}

//Face Code:
void MyGL::slot_faceSelectionChanged(QListWidgetItem * f) {
    if (Face* face = dynamic_cast<Face *>(f)) {
        this->faceSelection = face;
    }
}

//Vertex Code:
void MyGL::slot_vertexSelectionChanged(QListWidgetItem * v) {
    if (Vertex* vertex = dynamic_cast<Vertex *>(v)) {
        this->vertexSelection = vertex;
        emit sig_sendVertexX(double(vertex->position.x));
        emit sig_sendVertexY(double(vertex->position.y));
        emit sig_sendVertexZ(double(vertex->position.z));
    }
}
//Translation:
void MyGL::slot_receiveVertexTranslateX(double x) {
    if (vertexSelection != nullptr) {
        vertexSelection->position = glm::vec3(float(x),vertexSelection->position.y,vertexSelection->position.z);
    }
    meshes[0]->create();
}
void MyGL::slot_receiveVertexTranslateY(double y) {
    if (vertexSelection != nullptr) {
        vertexSelection->position = glm::vec3(vertexSelection->position.x,float(y),vertexSelection->position.z);
    }
    meshes[0]->create();
}
void MyGL::slot_receiveVertexTranslateZ(double z) {
    if (vertexSelection != nullptr) {
        vertexSelection->position = glm::vec3(vertexSelection->position.x,vertexSelection->position.y,float(z));
    }
    meshes[0]->create();
}

void MyGL::slot_selectionID(int id) {
    prog_mesh.setSelectedID(id);
}
