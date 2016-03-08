#include "nodetree.h"
#include <QEvent>
#include <QInputDialog>
#include <iostream>
#include <mygl.h>

NodeTree::NodeTree(QWidget* parent)  : QTreeWidget(parent) {

}

//Adding nodes to widget initially
void NodeTree::slot_receiveNode(QTreeWidgetItem *item) {
    QTreeWidget::addTopLevelItem(item);
}

int NodeTree::getSelectionIndex() {
    return QTreeWidget::indexOfTopLevelItem(selection);
}

void NodeTree:: slot_bindMeshButtonPressed() {
    emit sig_bindAtIndex(getSelectionIndex());
}


//Translations
void NodeTree:: slot_translateSelectedX(double tx) {

        if (selection != nullptr)
            selection->setPosition(new glm::vec3(tx,selection->getPosition()->y,selection->getPosition()->z));

        emit sig_updateVertices();

        selection->create(true);

}
void NodeTree:: slot_translateSelectedY(double ty) {

    if (selection != nullptr)
        selection->setPosition(new glm::vec3(selection->getPosition()->x,ty,selection->getPosition()->z));

    emit sig_updateVertices();

    selection->create(true);

}
void NodeTree:: slot_translateSelectedZ(double tz) {

    if (selection != nullptr)
        selection->setPosition(new glm::vec3(selection->getPosition()->x,selection->getPosition()->y,tz));

    emit sig_updateVertices();

    selection->create(true);

}

//Rotations
void NodeTree::slot_rotateXPlus() {
    if (selection != nullptr) {
        selection->setRotation(
                    new glm::quat(
                                  glm::angleAxis(5.0f,glm::vec3(1,0,0)) * *selection->getRotation()));
    }

    emit sig_updateVertices();

    selection->create(true);
}
void NodeTree::slot_rotateXMinus() {
    if (selection != nullptr) {
        selection->setRotation(
          new glm::quat(
                        glm::angleAxis(-5.0f,glm::vec3(1,0,0)) * *selection->getRotation()));
    }

    emit sig_updateVertices();

    selection->create(true);
}

void NodeTree::slot_rotateYPlus() {
    if (selection != nullptr) {
        selection->setRotation(new glm::quat(
                                   glm::angleAxis(5.0f,glm::vec3(0,1,0)) * *selection->getRotation()));
    }

    emit sig_updateVertices();

    selection->create(true);
}


void NodeTree::slot_rotateYMinus() {
    if (selection != nullptr) {
        selection->setRotation(new glm::quat(
                                   glm::angleAxis(-5.0f,glm::vec3(0,1,0)) * *selection->getRotation()));
    }

    emit sig_updateVertices();

    selection->create(true);
}

void NodeTree::slot_rotateZPlus() {
    if (selection != nullptr) {
        selection->setRotation(new glm::quat(
                                   glm::angleAxis(5.0f,glm::vec3(0,0,1)) * *selection->getRotation()));
    }

    emit sig_updateVertices();
    selection->create(true);
}

void NodeTree::slot_rotateZMinus() {
    if (selection != nullptr) {
        selection->setRotation(new glm::quat(
                                   glm::angleAxis(-5.0f,glm::vec3(0,0,1)) * *selection->getRotation()));
    }

    emit sig_updateVertices();
    selection->create(true);
}

//Adjust Spin Boxes when a node's selection is changed
void NodeTree::slot_selectionChanged(QTreeWidgetItem *item) {
    if (selection != nullptr) {
        selection->create(false);
    }
     selection = static_cast<Joint*>(item);

     glm::vec3 position = *(selection->getPosition());

     //Change SpinBox Values to reflect current transformations
     emit sig_rotateX(double(selection->getRotation()->x));
     emit sig_rotateY(double(selection->getRotation()->y));
     emit sig_rotateZ(double(selection->getRotation()->z));

     emit sig_translateX(position.x);
     emit sig_translateY(position.y);
     emit sig_translateZ(position.z);

     emit sig_selectionID(selection->getID());

     selection->create(true);

}

NodeTree::~NodeTree() {

}
